#include "parser/choice.h"
#include "parsers.h"
#include "json/number.h"
#include "json/string.h"
#include "json/types.h"
#include "json/whitespace.h"
#include <memory>

namespace json {
std::optional<ParserResult<Value>> bool_true(std::string_view input) {
  // clang-format off
  return map(
    joins(
      whitespace,
      parsers::string("true"),
      whitespace
    ),
    [](auto tuple) {
      auto [ws_before, got, ws_after] = std::move(tuple);
      return Value{
      std::make_unique<True>(True {got})
        , std::move(ws_before), std::move(ws_after)};
    }
  )(input);
  // clang-format on
}
std::optional<ParserResult<Value>> bool_false(std::string_view input) {
  // clang-format off
  return map(
    joins(
      whitespace,
      parsers::string("false"),
      whitespace
    ),
    [](auto tuple) {
      auto [ws_before, got, ws_after] = std::move(tuple);
      auto t = std::make_unique<False>(False {got});
      return Value{std::move(t), std::move(ws_before), std::move(ws_after)};
    }
  )(input);
  // clang-format on
}
std::optional<ParserResult<Value>> null(std::string_view input) {
  // clang-format off
  return map(
    joins(
      whitespace,
      parsers::string("null"),
      whitespace
    ),
    [](auto tuple) {
      auto [ws_before, got, ws_after] = std::move(tuple);
      auto t = std::make_unique<Null>(Null {got});
      return Value{std::move(t), std::move(ws_before), std::move(ws_after)};
    }
  )(input);
  // clang-format on
}

template <class T>
static Value base_mapper(std::tuple<Whitespace, T, Whitespace> tuple) {
  auto [ws_before, got, ws_after] = std::move(tuple);
  return Value{std::make_unique<T>(std::move(got)), std::move(ws_before),
               std::move(ws_after)};
}

std::optional<ParserResult<Value>> value(std::string_view input) {
  // clang-format off
   
  return parsers::choices(
    parsers::map(
      parsers::joins(
        whitespace,
        json::string,
        whitespace
      ),
      base_mapper<String>
    ),
    parsers::map(
      parsers::joins(
        whitespace,
        json::number,
        whitespace
      ),
      base_mapper<Number>
    ),
    json::bool_true,
    json::bool_false,
    json::null
  )(input);
}
} // namespace json