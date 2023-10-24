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
    auto result = std::move(parser(input));

    if (result.has_value()) {
      auto value = std::move(result.value());

      auto value_remaining = value.remaining;
      return ParserResult<MapperReturnType<decltype(parser), decltype(mapper)>>{
          std::move(mapper(std::move(value.value))),
          value_remaining,
      };
    }

    return std::nullopt;
  };
}

} // namespace parsers

#endif // PARSER_MAP_H