#ifndef PARSER_DIGITS_H
#define PARSER_DIGITS_H

#include "parser.h"
#include <cerrno>
#include <charconv>
#include <stdexcept>
#include <string>

namespace parsers {

std::optional<ParserResult<int>> digits(std::string_view input) {
  if (input.empty())
    return std::nullopt;

  int value;
  auto start_addr = std::to_address(input.cbegin());
  auto end_addr = std::to_address(input.cend());
  auto [ptr, ec] = std::from_chars(start_addr, end_addr, value);
  if (ec != std::errc{}) {
    // 失敗した場合
    return std::nullopt;
  }

  std::string_view rest(ptr, (end_addr - ptr) / sizeof(*ptr));
  return ParserResult<int>{value, rest};
}

} // namespace parsers
#endif // PARSER_DIGITS_H