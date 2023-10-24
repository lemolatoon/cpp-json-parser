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


template <class T, class U>
struct OriginalPair {
  T first;
  U second;
};
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
            return OriginalPair<std::string_view, std::vector<Value>>{std::string_view{input.data(), length - 1 /* remove of trailing comma */}, std::move(elems)};
          }
        ),
        parsers::map(
          whitespace,
          [&](auto ws) {
            return OriginalPair<std::string_view, std::vector<Value>>{ws.original(), std::move(std::vector<Value>{})};
          }
        )
      ),
      parsers::character(']')
    ),
    [&](std::tuple<char, OriginalPair<std::string_view, std::vector<Value>>, char> tuple) {
      auto [left, inner, right] = std::move(tuple);
      auto view = std::move(inner.first);
      auto elems = std::move(inner.second);
      return Array{std::string_view{input.data(), 2 + view.size()}, std::move(elems)};
    }
  )(input);
}

} // namespace json