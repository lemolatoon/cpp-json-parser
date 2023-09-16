#ifndef PARSER_JOIN_H
#define PARSER_JOIN_H
#include "parser.h"
#include "parser/map.h"
#include <optional>

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

} // namespace parsers

#endif // PARSER_JOIN_H