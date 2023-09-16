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
template <class T, class U>
Parser<std::vector<T>> auto separated(Parser<T> auto parser,
                                      Parser<U> auto separator) {
  return [=](std::string_view input)
             -> std::optional<ParserResult<std::vector<T>>> {
    auto results = std::vector<T>(0);

    auto result = parser(input);
    if (!result.has_value())
      return ParserResult<std::vector<T>>{results, input};
    auto value = result.value();
    results.push_back(value.value);
    input = value.remaining;

    while (true) {
      auto separator_result = separator(input);
      if (!separator_result.has_value())
        break;

      auto result = parser(separator_result.value().remaining);
      // 末尾がseparatorで終わってしまってる。
      if (!result.has_value())
        return std::nullopt;

      auto value = result.value();
      results.push_back(value.value);

      input = value.remaining;
    }

    return ParserResult<std::vector<T>>{results, input};
  };
}

} // namespace parsers

#endif // PARSER_SEPARATED_H