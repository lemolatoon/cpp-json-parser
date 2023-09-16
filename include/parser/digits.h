#ifndef PARSER_DIGITS_H
#define PARSER_DIGITS_H

#include "parser.h"
#include <string>

namespace parsers {

std::optional<ParserResult<int>> digits(std::string_view input);

} // namespace parsers
#endif // PARSER_DIGITS_H