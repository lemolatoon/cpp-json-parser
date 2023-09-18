#ifndef PARSER_SEPARATED_H
#define PARSER_SEPARATED_H
#include "parser.h"
#include <optional>
#include <vector>

namespace parsers {

/**
 * パーサーを２つ受けとり、parser, separatorで交互にパースし、
 * separatorがパースできなくなるまで繰り返し、parserの結果を
 * std::vectorに詰めて返す。
 */
template <bool Trailing = false>
auto inline separated(Parser auto parser, Parser auto separator)
    -> Parser<std::vector<ParserReturnType<decltype(parser)>>> auto{
  return
      [=](std::string_view input)
          -> std::optional<
              ParserResult<std::vector<ParserReturnType<decltype(parser)>>>> {
        auto results = std::vector<ParserReturnType<decltype(parser)>>(0);

        auto result = parser(input);
        if (!result.has_value())
          return ParserResult<std::vector<ParserReturnType<decltype(parser)>>>{
              results, input};
        auto value = result.value();
        results.push_back(value.value);
        input = value.remaining;

        while (true) {
          auto separator_result = separator(input);
          if (!separator_result.has_value())
            break;

          auto separator_value = separator_result.value();
          auto result = parser(separator_value.remaining);

          // 末尾がseparatorで終わってしまってる。
          if (!result.has_value()) {
            if constexpr (Trailing) {
              // Trailingがtrueのときは、最後のseparatorまで読み込み、結果を返す。
              input = separator_value.remaining;
              break;
            } else {
              // そうでないときは、無効な入力なので、std::nulloptを返す。
              return std::nullopt;
            }
          }

          auto value = result.value();
          results.push_back(value.value);

          input = value.remaining;
        }

        return ParserResult<std::vector<ParserReturnType<decltype(parser)>>>{
            results, input};
      };
}

} // namespace parsers

#endif // PARSER_SEPARATED_H