#ifndef PARSER_JOIN_H
#define PARSER_JOIN_H
#include "parser.h"
#include "parser/map.h"
#include <optional>
#include <tuple>

namespace parsers {

/**
 *  パーサー２つを受けとり、１つ目でパースが`std::nullopt`でないときは、
 *  そのremainingにさらに２つ目のパーサーを適用して結果のvalueのstd::pairを返す。
 *  ２つ目のパーサーの結果が`std::nullopt`なら、`std::nullopt`を返す。
 */
template <class T, class U>
Parser<std::pair<T, U>> auto inline join(Parser<T> auto primary_parser,
                                         Parser<U> auto secondary_parser) {
  return [=](std::string_view input)
             -> std::optional<ParserResult<std::pair<T, U>>> {
    auto result = primary_parser(input);

    if (!result.has_value())
      return std::nullopt;

    auto value = result.value();

    auto result2 = secondary_parser(value.remaining);

    if (!result2.has_value())
      return std::nullopt;

    auto value2 = result2.value();

    return ParserResult<std::pair<T, U>>{{value.value, value2.value},
                                         value2.remaining};
  };
}

/*
 * 引数で与えられた可変長個のパーサーを順番に適用し、途中で一つでも
 * `std::nullopt` を返せば、`std::nullopt`
 * を、すべてパース結果が値を持っていれば、その値たちのタプルを返す。
 *
 * ほとんどの実装は やいとさん(@yaito3014) によるもの。
 */
auto inline joins(Parser auto primary_parser, Parser auto... secondary_parsers)
    -> Parser<
        std::tuple<ParserReturnType<decltype(primary_parser)>,
                   ParserReturnType<decltype(secondary_parsers)>...>> auto {
  return [=](std::string_view input)
             -> std::optional<ParserResult<std::tuple<
                 ParserReturnType<decltype(primary_parser)>,
                 ParserReturnType<decltype(secondary_parsers)>...>>> {
    auto result = primary_parser(input);

    if (!result.has_value())
      return std::nullopt;

    auto value = result.value();

    if constexpr (sizeof...(secondary_parsers) > 0) {
      auto rest = joins(secondary_parsers...)(value.remaining);

      if (!rest.has_value())
        return std::nullopt;

      auto rest_value = rest.value();

      return ParserResult<
          std::tuple<ParserReturnType<decltype(primary_parser)>,
                     ParserReturnType<decltype(secondary_parsers)>...>>{
          std::tuple_cat(std::tuple{value.value}, rest_value.value),
          rest_value.remaining};
    } else {
      return ParserResult<
          std::tuple<ParserReturnType<decltype(primary_parser)>,
                     ParserReturnType<decltype(secondary_parsers)>...>>{
          std::tuple{value.value}, value.remaining};
    }
  };
}

} // namespace parsers

#endif // PARSER_JOIN_H