#ifndef PARSER_STRING_H
#define PARSER_STRING_H
#include "parser.h"
#include <optional>
#include <string>

namespace parsers {

Parser<std::string_view> auto string(std::string_view literal) {
  return [=](std::string_view input)
             -> std::optional<ParserResult<std::string_view>> {
    if (input.empty() || input.size() < literal.size())
      return std::nullopt;

    if (input.starts_with(literal)) {
      return ParserResult<std::string_view>{input.substr(0, literal.size()),
                                            input.substr(literal.size())};
    }
    return std::nullopt;
  };
}

} // namespace parsers

#endif // PARSER_STRING_H