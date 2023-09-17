#ifndef PARSER_MAP_H
#define PARSER_MAP_H
#include "parser.h"
#include <optional>
#include <type_traits>

namespace parsers {

/*
 * mapper(T) -> U
 */
template <class F, class T>
concept Mapper = std::is_invocable_v<F, T>;

template <Parser P, Mapper<ParserReturnType<P>> M>
using MapperReturnType = std::invoke_result<M, ParserReturnType<P>>::type;

/**
 *  パーサーとマッパーを受けとり、パーサーの結果をマップして返すパーサーを返す。
 */
auto inline map(Parser auto parser,
                Mapper<ParserReturnType<decltype(parser)>> auto mapper)
    -> Parser<MapperReturnType<decltype(parser), decltype(mapper)>> auto{
  return [=](std::string_view input)
             -> std::optional<ParserResult<
                 MapperReturnType<decltype(parser), decltype(mapper)>>> {
    auto result = parser(input);

    if (result.has_value()) {
      auto value = result.value();

      return ParserResult<MapperReturnType<decltype(parser), decltype(mapper)>>{
          mapper(value.value),
          value.remaining,
      };
    }

    return std::nullopt;
  };
}

} // namespace parsers

#endif // PARSER_MAP_H