#ifndef JSON_NUMBER_H
#define JSON_NUMBER_H

#include "parser.h"
#include "types.h"

namespace json {

std::optional<ParserResult<Number>> number(std::string_view input);

std::optional<ParserResult<std::string_view>> integer(std::string_view input);

std::optional<ParserResult<std::string_view>> digits(std::string_view input);

std::optional<ParserResult<char>> digit(std::string_view input);

std::optional<ParserResult<char>> onenine(std::string_view input);

std::optional<ParserResult<std::optional<std::string_view>>>
fraction(std::string_view input);

std::optional<ParserResult<std::optional<std::string_view>>>
exponent(std::string_view input);

std::optional<ParserResult<std::string_view>> sign(std::string_view input);

} // namespace json

#endif // JSON_NUMBER_H