#ifndef JSON_OBJECT_H
#define JSON_OBJECT_H

#include "unicode/unistr.h"
#include "json/types.h"
#include <map>

namespace json {

std::optional<ParserResult<Object>> object(std::string_view input);

std::optional<ParserResult<parser_utils::UnfoldablePair<
    std::string_view, parser_utils::UnfoldablePair<icu::UnicodeString, Value>>>>
member(std::string_view input);

std::optional<ParserResult<parser_utils::UnfoldablePair<
    std::string_view, std::map<icu::UnicodeString, Value>>>>
members(std::string_view input);

} // namespace json

#endif // JSON_OBJECT_H