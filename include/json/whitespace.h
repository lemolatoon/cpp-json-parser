#ifndef JSON_WHITESPACE_H
#define JSON_WHITESPACE_H

#include "parser.h"
#include <optional>
#include <string>

namespace json {

std::optional<ParserResult<Unit>> whitespace(std::string_view input);

} // namespace json

#endif // JSON_WHITESPACE_H