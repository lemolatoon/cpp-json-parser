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
template <class T>
Parser<std::vector<T>> auto inline many(Parser<T> auto parser) {
  return [=](std::string_view input)
             -> std::optional<ParserResult<std::vector<T>>> {
    auto results = std::vector<T>(0);
    while (true) {
      auto result = parser(input);
      if (!result.has_value())
        break;

      auto value = result.value();
      results.push_back(value.value);

      input = value.remaining;
    }

    return ParserResult<std::vector<T>>{results, input};
  };
}

} // namespace parsers

#endif // PARSER_MANY_H