#ifndef PARSER_CHOICE_H
#define PARSER_CHOICE_H
#include "parser.h"
#include <optional>

namespace parsers {

/**
 *  パーサーと２つを受けとり、１つ目でパースが`std::nullopt`でないときは、その結果を、
 *  できなかったときは２つ目のパーサーの結果を返す。
 */
template <class T>
Parser<T> auto inline choice(Parser<T> auto primary_parser,
                             Parser<T> auto secondary_parser) {
  return [=](std::string_view input) -> std::optional<ParserResult<T>> {
    auto result = primary_parser(input);

    if (result.has_value()) {
      return result;
    }

    return secondary_parser(input);
  };
}

} // namespace parsers

#endif // PARSER_CHOICE_H