#include "json/array.h"
#include "parser/separated.h"
#include "parsers.h"
#include "json/value.h"
#include "json/whitespace.h"
#include <numeric>

namespace json {

std::optional<ParserResult<Value>> element(std::string_view input) {
  return json::value(input);
}
std::optional<ParserResult<std::deque<Value>>>
elements(std::string_view input) {
  // clang-format off
  return parsers::choices(
    parsers::map(
      parsers::joins(
        json::element,
        parsers::character(','),
        json::elements
      ),
      [&](std::tuple<Value, char, std::deque<Value>> tuple) {
        auto [first, comma, rest] = std::move(tuple);
        rest.push_front(std::move(first));
        return std::move(rest);
      }
    ),
    parsers::map(
      json::element,
      [&](auto elem) {
        auto deq = std::deque<Value>{};
        deq.push_back(std::move(elem));
        return std::move(deq);
      }
    )
  )(input);
  // clang-format on
}

template <class T, class U> struct OriginalPair {
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
          [&](std::deque<Value> elems_deque) {
            size_t length = 0;

            // std::deque to std::vector
            std::vector<Value> elems;
            for (auto it = elems_deque.begin(); it != elems_deque.end(); ++it) {
              elems.emplace_back(std::move(*it));
            }

            for (const auto& elem : elems) {
              length += elem.original().size() + 1 /* comma */;
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