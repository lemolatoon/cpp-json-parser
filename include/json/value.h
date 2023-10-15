#ifndef JSON_VALUE_H
#define JSON_VALUE_H

#include "json/types.h"
namespace json {

std::optional<ParserResult<True>> bool_true(std::string_view input);
std::optional<ParserResult<False>> bool_false(std::string_view input);
std::optional<ParserResult<Null>> bool_null(std::string_view input);

std::optional<ParserResult<Value>> value(std::string_view input);

} // namespace json

#endif // JSON_VALUE_H