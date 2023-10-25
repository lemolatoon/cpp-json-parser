#include "json/types.h"
#include <map>

#include "parser/character.h"
#include "parser/choice.h"
#include "parsers.h"
#include "unicode/unistr.h"
#include "json/array.h"
#include "json/object.h"
#include "json/string.h"
#include "json/value.h"
#include "json/whitespace.h"
#include <string>

namespace json {

std::optional<ParserResult<parser_utils::UnfoldablePair<
    std::string_view, parser_utils::UnfoldablePair<icu::UnicodeString, Value>>>>
member(std::string_view input) {
  // clang-format off
  return parsers::map(
    parsers::joins(
      json::whitespace,
      json::string,
      json::whitespace,
      parsers::character(':'),
      json::element
    ),
    [&](std::tuple<Whitespace, String, Whitespace, char, Value> tuple) {
      auto [ws_before, key, ws_after, colon, value] = std::move(tuple);
      auto original = std::string_view{input.data(), ws_before.original().size() + key.original().size() + ws_after.original().size() + 1 /* colon */ + value.original().size()};
      return parser_utils::UnfoldablePair<std::string_view, parser_utils::UnfoldablePair<icu::UnicodeString, Value>>{
        original,
        parser_utils::UnfoldablePair<icu::UnicodeString, Value>{
          icu::UnicodeString(key.unicode_string()),
          std::move(value)
        }
      };
    }
  )(input);
  // clang-format on
}

std::optional<ParserResult<parser_utils::UnfoldablePair<
    std::string_view, std::map<icu::UnicodeString, Value>>>>
members(std::string_view input) {
  // clang-format off
  return parsers::choices(
    parsers::map(
      parsers::joins(
        json::member,
        parsers::character(','),
        json::members
      ),
      [&](std::tuple<parser_utils::UnfoldablePair<std::string_view, parser_utils::UnfoldablePair<icu::UnicodeString, Value>>, char, parser_utils::UnfoldablePair<std::string_view, std::map<icu::UnicodeString, Value>>> tuple) {
        auto [first, comma, rest] = std::move(tuple);
        auto first_original = first.first;
        auto first_key_value = std::move(first.second);
        rest.second.insert(std::make_pair(first_key_value.first, std::move(first_key_value.second)));
        auto original = std::string_view{input.data(), first_original.size() + 1 /* comma */ + rest.first.size()};
        return parser_utils::UnfoldablePair<std::string_view, std::map<icu::UnicodeString, Value>> {
          original,
          std::move(rest.second)
        };
      }
    ),
    parsers::map(
      json::member,
      [&](auto elem) {
        auto map = std::map<icu::UnicodeString, Value>{};
        auto original = elem.first;
        map.insert(std::make_pair(elem.second.first, std::move(elem.second.second)));
        return parser_utils::UnfoldablePair<std::string_view, std::map<icu::UnicodeString, Value>> {
          original,
          std::move(map)
        };
      }
    )
  )(input);
  // clang-format on
}

std::optional<ParserResult<Object>> object(std::string_view input) {
  // clang-format off
  return map(
    parsers::joins(
      parsers::character('{'),
      parsers::choices(
        json::members,
        parsers::map(
          json::whitespace,
          [](auto ws) {
            auto map = std::map<icu::UnicodeString, Value>{};
            return parser_utils::UnfoldablePair<std::string_view, std::map<icu::UnicodeString, Value>>{ws.original(), std::move(map)};
          }
        )
      ),
      parsers::character('}')
    ),
    [&](auto tuple) {
      auto [open_brace, members, close_brace] = std::move(tuple);
      auto [members_original, members_map] = std::move(members);
      auto original = std::string_view{input.data(), members_original.size() + 2 /* braces */};
      return Object{original, std::move(members_map)};
    }
  )(input);
  // clang-format on
}
} // namespace json