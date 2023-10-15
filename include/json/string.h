#ifndef JSON_STRING_H
#define JSON_STRING_H

#include "parsers.h"
#include "json/types.h"
#include <optional>

#define DEFINE_ESCAPED_CHAR_PARSER_FN(name, ch)                                \
  inline std::optional<ParserResult<char>> name(std::string_view input) {      \
    return std::get<1>((parsers::join(parsers::character('\\'),                \
                                      parsers::character(ch)))(input));        \
  }

namespace json {

std::optional<ParserResult<String>> string(std::string_view input);

std::optional<ParserResult<std::pair<std::string_view, char16_t>>>
escape(std::string_view input);

std::optional<ParserResult<char>> hex(std::string_view input);

std::optional<ParserResult<std::pair<std::string_view, char16_t>>>
any_characters_except_double_quote_and_backslash(std::string_view input);

} // namespace json

#endif // JSON_STRING_H