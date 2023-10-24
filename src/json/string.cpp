#include "json/string.h"
#include "parsers.h"
#include "unicode/unistr.h"
#include "json/types.h"
#include <cstdint>
#include <numeric>
#include <string_view>
#include <utility>

using namespace parsers;

namespace json {

std::optional<ParserResult<String>> string(std::string_view input) {
  auto inner_content_mapper =
      [&](std::vector<std::pair<std::string_view, char16_t>> v) -> String {
    auto [original, content] = std::accumulate(
        v.begin(), v.end(),
        std::make_pair(input.substr(0, 0), icu::UnicodeString{}),
        [&](auto acc, auto pair) {
          auto [original1, content] = acc;
          auto [original2, ch] = pair;
          return std::make_pair(
              std::string_view{input.data(),
                               original1.size() + original2.size()},
              content + ch);
        });
    return String{original, content};
  };
  auto outer_mapper = [&](std::tuple<char, String, char> tuple) -> String {
    auto [prefix, str, suffix] = std::move(tuple);
    return String{std::string_view{input.data(), 2 + str.original().size()},
                  str.unicode_string()};
  };
  // clang-format off
  return map(
    joins(
        character('\"'),
        map(
            many(
                choice(
                    any_characters_except_double_quote_and_backslash,
                    escape
                )
            ),
            inner_content_mapper
        ),
        character('\"')
    ),
    outer_mapper
  )(input);
  // clang-format on
} // namespace json

std::optional<ParserResult<std::pair<std::string_view, char16_t>>>
escape(std::string_view input) {
  auto into_codepoint = [&](auto pair) {
    auto [prefix, tuple] = pair;
    auto [a1, a2, a3, a4] = tuple;
    auto into_int = [](auto hexchar) {
      return (hexchar >= 'A' && hexchar <= 'E')   ? (hexchar - 'A' + 10)
             : (hexchar >= 'a' && hexchar <= 'e') ? (hexchar - 'a' + 10)
                                                  : (hexchar - '0');
    };
    char16_t value = into_int(a1) * 4096 + into_int(a2) * 256 +
                     into_int(a3) * 16 + into_int(a4);
    auto original = input.substr(0, 5);
    return std::make_pair(original, value);
  };

  auto backslash_prefixer = [&](auto joined) {
    auto [prefix, pair] = joined;
    auto [original, codepoint] = pair;
    return std::make_pair(std::string_view{input.data(), original.size() + 1},
                          codepoint);
  };
  auto mapper = [&](char ch) {
    char converted = '!';
    switch (ch) {
    case '\"':
      converted = '\"';
      break;
    case '\\':
      converted = '\\';
      break;
    case '/':
      converted = '/';
      break;
    case 'b':
      converted = '\b';
      break;
    case 'f':
      converted = '\f';
      break;
    case 'n':
      converted = '\n';
      break;
    case 'r':
      converted = '\r';
      break;
    case 't':
      converted = '\t';
      break;
    }

    return std::make_pair(input.substr(0, 1), static_cast<char16_t>(converted));
  };
  // clang-format off
    return map(
        join(
            character('\\'),
            choice(
                map(choices(
                    character('\"'),
                    character('\\'),
                    character('/'),
                    character('b'),
                    character('f'),
                    character('n'),
                    character('r'),
                    character('t')
                ), mapper),
                map(join(
                    character('u'),
                    joins(
                        hex, 
                        hex,
                        hex,
                        hex
                    )
                ), into_codepoint)
            )
        ),
        backslash_prefixer
    )(input);
  // clang-format on
}

std::optional<ParserResult<char>> hex(std::string_view input) {
  // clang-format off
  return choices(
    character('0'),
    character('1'),
    character('2'),
    character('3'),
    character('4'),
    character('5'),
    character('6'),
    character('7'),
    character('8'),
    character('9'),
    character('a'),
    character('b'),
    character('c'),
    character('d'),
    character('e'),
    character('f'),
    character('A'),
    character('B'),
    character('C'),
    character('D'),
    character('E'),
    character('F')
  )(input);
  // clang-format on
}

std::optional<ParserResult<std::pair<std::string_view, char16_t>>>
any_characters_except_double_quote_and_backslash(std::string_view input) {
  if (input.empty() || input[0] == '\"' || input[0] == '\\') {
    return std::nullopt;
  }
  auto unicode_string = icu::UnicodeString::fromUTF8(input);
  int32_t index = 0;

  auto next = unicode_string[0];
  // TODO: ここで文字列の長さを取得するのは非効率的
  std::string rest;
  unicode_string.tempSubString(0, 1).toUTF8String(rest);

  auto remaining = input.substr(rest.size());

  auto value = std::make_pair(rest, next);
  return ParserResult<std::pair<std::string_view, char16_t>>{value, remaining};
}

} // namespace json