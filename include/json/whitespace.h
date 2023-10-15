#ifndef JSON_WHITESPACE_H
#define JSON_WHITESPACE_H

#include "parser.h"
#include "json/types.h"
#include <optional>
#include <string>

namespace json {

std::optional<ParserResult<Whitespace>> whitespace(std::string_view input);

} // namespace json

#endif // JSON_WHITESPACE_H