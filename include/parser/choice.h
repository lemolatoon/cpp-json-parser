#ifndef PARSER_CHOICE_H
#define PARSER_CHOICE_H
#include "parser.h"
#include <concepts>
#include <optional>
#include <tuple>
#include <type_traits>

namespace parsers {

/**
 *  パーサーと２つを受けとり、１つ目でパースが`std::nullopt`でないときは、その結果を、
 *  できなかったときは２つ目のパーサーの結果を返す。
 */
auto inline choice(Parser auto primary_parser, Parser auto secondary_parser)
    -> Parser<ParserReturnType<decltype(primary_parser)>> auto{
  static_assert(std::is_same_v<ParserReturnType<decltype(primary_parser)>,
                               ParserReturnType<decltype(secondary_parser)>>);

  return [=](std::string_view input)
             -> std::optional<
                 ParserResult<ParserReturnType<decltype(primary_parser)>>> {
    auto result = primary_parser(input);

    if (result.has_value()) {
      return result;
    }

    return secondary_parser(input);
  };
}

template <typename T, typename... Ts>
concept are_same = std::conjunction_v<std::is_same<T, Ts>...>;

auto inline choices(Parser auto primary_parser, Parser auto... parsers_rest)
    -> Parser<ParserReturnType<decltype(primary_parser)>> auto{
  // assert all parsers' ReturnType are same
  static_assert(are_same<ParserReturnType<decltype(primary_parser)>,
                         ParserReturnType<decltype(parsers_rest)>...>);

  return choices_inner(primary_parser, parsers_rest...);
}

static auto inline choices_inner(Parser auto primary_parser,
                                 Parser auto... parsers_rest) {
  return [=](std::string_view input)
             -> std::optional<
                 ParserResult<ParserReturnType<decltype(primary_parser)>>> {
    auto result = primary_parser(input);
    if (result.has_value())
      return result;

    if constexpr (sizeof...(parsers_rest) > 0) {
      return choices_inner(parsers_rest...)(input);
    } else {
      return std::nullopt;
    }
  };
}

} // namespace parsers

#endif // PARSER_CHOICE_H