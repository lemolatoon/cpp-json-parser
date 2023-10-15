#include "json/array.h"
#include "parser/separated.h"
#include "parsers.h"
#include "json/value.h"
#include "json/whitespace.h"
#include <numeric>

namespace json {

std::optional<ParserResult<Value>> element(std::string_view input);
std::optional<ParserResult<std::vector<Value>>>
elements(std::string_view input);

std::optional<ParserResult<Array>> array(std::string_view input) {
  // clang-format off
  return map(
    joins(
      parsers::character('['),
      choices(
        parsers::map(
          elements,
          [&](auto elems) {
            size_t length = 0;
            for (auto &elem : elems) {
              // + 1 for the comma
              length += elem.original().size() + 1;
            }
            return std::make_pair(std::string_view{input.data(), length - 1 /* remove of trailing comma */}, elems);
          }
        ),
        parsers::map(
          whitespace,
          [&](auto ws) {
            return std::make_pair(ws.original(), std::vector<Value>{});
          }
        )
      ),
      parsers::character(']')
    ),
    [&](std::tuple<char, std::pair<std::string_view, std::vector<Value>>, char> tuple) {
      auto [left, inner, right] = tuple;
      auto [view, elems] = inner;
      return Array{std::string_view{input.data(), 2 + view.size()}, elems};
    }
  )(input);
}

} // namespace json