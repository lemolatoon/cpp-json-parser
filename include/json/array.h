#ifndef JSON_ARRAY_H
#define JSON_ARRAY_H

#include "json/types.h"
#include <deque>
#include <optional>
#include <string>

namespace json {

std::optional<ParserResult<Array>> array(std::string_view input);
std::optional<ParserResult<Value>> element(std::string_view input);
std::optional<ParserResult<std::deque<Value>>> elements(std::string_view input);

} // namespace json

#endif // JSON_ARRAY_H