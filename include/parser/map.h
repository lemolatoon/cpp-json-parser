#ifndef PARSER_MAP_H
#define PARSER_MAP_H
#include "parser.h"
#include <optional>

namespace parsers {

/*
 * mapper(T) -> U
 */
template <class F, class T, class U>
concept Mapper = std::is_invocable_r_v<U, F, T>;

/**
 *  パーサーとマッパーを受けとり、パーサーの結果をマップして返すパーサーを返す。
 */
template <class T, class U>
Parser<U> auto map(Parser<T> auto parser, Mapper<T, U> auto mapper) {
  return [=](std::string_view input) -> std::optional<ParserResult<T>> {
    auto result = parser(input);

    if (result.has_value()) {
      auto value = result.value();

      return ParserResult<T>{
          mapper(value.value),
          value.remaining,
      };
    }

    return std::nullopt;
  };
}

} // namespace parsers

#endif // PARSER_MAP_H