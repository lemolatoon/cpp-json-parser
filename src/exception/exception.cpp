#include "parsers.h"
#include <algorithm>
#include <exception.h>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>

const char *ParserException::what() const noexcept { return message_.data(); }

const std::string
ParserException::error_message(std::string_view original_input) const {
  auto lines =
      parsers::separated<true>(REGEX(R"(([^\n])*)"),
                               parsers::character('\n'))(original_input)
          .value()
          .value;

  auto input_rest_start_address = std::to_address(input_rest_.cbegin());
  auto error_at = std::find_if(lines.cbegin(), lines.cend(), [&](auto line) {
    auto line_start_address = std::to_address(line.cbegin());
    auto line_end_address = std::to_address(line.cend());

    return line_start_address <= input_rest_start_address &&
           input_rest_start_address <= line_end_address;
  });

  if (error_at == lines.cend()) {
    // なぜか、パースできなくて残った部分である`input_rest`が、`original_input`に含まれていない。
    // 単純にエラー文を返す。
    return message_;
  }

  auto error_line = std::string(*error_at);
  auto distance = std::distance((*error_at).cbegin(), input_rest_.cbegin());

  std::string error_message;

  error_message += (error_line + "\n");
  error_message += std::string(distance, ' ') + "^" + "\n";
  error_message += std::string(distance, ' ') + message_;

  return error_message;
}