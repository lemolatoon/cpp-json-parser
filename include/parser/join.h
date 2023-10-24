#ifndef PARSER_JOIN_H
#define PARSER_JOIN_H
#include "parser.h"
#include <optional>
#include <tuple>

namespace parsers {

/**
 *  パーサー２つを受けとり、１つ目でパースが`std::nullopt`でないときは、
 *  そのremainingにさらに２つ目のパーサーを適用して結果のvalueのstd::pairを返す。
 *  ２つ目のパーサーの結果が`std::nullopt`なら、`std::nullopt`を返す。
 */
auto inline join(Parser auto primary_parser, Parser auto secondary_parser)
    -> Parser<std::pair<ParserReturnType<decltype(primary_parser)>,
                        ParserReturnType<decltype(secondary_parser)>>> auto{
  return [=](std::string_view input)
             -> std::optional<ParserResult<
                 std::pair<ParserReturnType<decltype(primary_parser)>,
                           ParserReturnType<decltype(secondary_parser)>>>> {
    auto result = std::move(primary_parser(input));

    if (!result.has_value())
      return std::nullopt;

    auto value = std::move(result.value());

    auto result2 = std::move(secondary_parser(value.remaining));

    if (!result2.has_value())
      return std::nullopt;

    auto value2 = std::move(result2.value());

    auto value2_remaining = value2.remaining;
    return ParserResult<
        std::pair<ParserReturnType<decltype(primary_parser)>,
                  ParserReturnType<decltype(secondary_parser)>>>{
        {std::move(value.value), std::move(value2.value)}, value2_remaining};
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
                   ParserReturnType<decltype(secondary_parsers)>...>> auto{
  return
      [=](std::string_view input)
          -> std::optional<ParserResult<
              std::tuple<ParserReturnType<decltype(primary_parser)>,
                         ParserReturnType<decltype(secondary_parsers)>...>>> {
        auto result = std::move(primary_parser(input));

        if (!result.has_value())
          return std::nullopt;

        auto value = std::move(result.value());

        if constexpr (sizeof...(secondary_parsers) > 0) {
          auto rest = std::move(joins(secondary_parsers...)(value.remaining));

          if (!rest.has_value())
            return std::nullopt;

          auto rest_value = std::move(rest.value());

          auto rest_remaining = rest_value.remaining;
          return ParserResult<
              std::tuple<ParserReturnType<decltype(primary_parser)>,
                         ParserReturnType<decltype(secondary_parsers)>...>>{
              std::move(std::tuple_cat(std::tuple{std::move(value.value)},
                                       std::move(rest_value.value))),
              rest_remaining};
        } else {
          auto value_remaining = value.remaining;
          return ParserResult<
              std::tuple<ParserReturnType<decltype(primary_parser)>,
                         ParserReturnType<decltype(secondary_parsers)>...>>{
              std::tuple{std::move(value.value)}, value_remaining};
        }
      };
}

} // namespace parsers

#endif // PARSER_JOIN_H