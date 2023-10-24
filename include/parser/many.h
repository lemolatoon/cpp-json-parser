#ifndef PARSER_MANY_H
#define PARSER_MANY_H
#include "parser.h"
#include <optional>
#include <vector>

namespace parsers {

/**
 *  パーサーを受けとり、そのパーサーで0回以上パースを試みる。
 *  パース結果を`std::vector`に詰めて返す。
 */
auto inline many(Parser auto parser)
    -> Parser<std::vector<ParserReturnType<decltype(parser)>>> auto{
  return
      [=](std::string_view input)
          -> std::optional<
              ParserResult<std::vector<ParserReturnType<decltype(parser)>>>> {
        auto results = std::vector<ParserReturnType<decltype(parser)>>(0);
        while (true) {
          auto result = std::move(parser(input));
          if (!result.has_value())
            break;

          auto value = std::move(result.value());
          results.push_back(value.value);

          input = value.remaining;
        }

        return ParserResult<std::vector<ParserReturnType<decltype(parser)>>>{
            std::move(results), input};
      };
}

} // namespace parsers

#endif // PARSER_MANY_H