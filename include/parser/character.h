#ifndef PARSER_CHARACTER_H
#define PARSER_CHARACTER_H
#include "parser.h"
#include <cassert>
#include <optional>

namespace parsers {

Parser<char> auto inline character(char c) {
  return [=](std::string_view input) -> std::optional<ParserResult<char>> {
    if (input.empty())
      return std::nullopt;
    if (input.front() == c)
      return ParserResult<char>{c, input.substr(1)};
    return std::nullopt;
  };
}

inline std::optional<ParserResult<std::string_view>>
empty(std::string_view input) {
  return ParserResult<std::string_view>{input.substr(0, 0), input};
}

} // namespace parsers

#endif // PARSER_CHARACTER_H