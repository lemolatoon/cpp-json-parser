#ifndef PARSER_CHOICE_H
#define PARSER_CHOICE_H
#include "parser.h"
#include <concepts>
#include <optional>
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

} // namespace parsers

#endif // PARSER_CHOICE_H