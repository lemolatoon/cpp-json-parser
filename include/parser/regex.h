#ifndef PARSER_REGEX_H
#define PARSER_REGEX_H
#include "parser.h"
#include <iterator>
#include <memory>
#include <optional>
#include <regex>
#include <string>

namespace parsers {

/*
 * regexを受け取り、マッチした部分を返し、そのマッチ部分より後のstring_viewをreaming
 * とするようなパーサーを返す。
 */
Parser<std::string_view> auto inline regex(std::regex re) {
  return [=](std::string_view input)
             -> std::optional<ParserResult<std::string_view>> {
    std::match_results<typename decltype(input)::const_iterator> match;
    if (!std::regex_search(input.cbegin(), input.cend(), match, re))
      return std::nullopt;

    auto first_match = match[0];
    if (!first_match.matched)
      return std::nullopt;

    auto matched_view = std::string_view(first_match.first, first_match.second);

    auto remaining_start_index = first_match.second - std::begin(input);
    auto remaining = std::string_view(input.substr(remaining_start_index));

    return ParserResult<std::string_view>{matched_view, remaining};
  };
}

} // namespace parsers

#define REGEX(regex_literal)                                                   \
  ([] {                                                                        \
    static std::regex re((regex_literal));                                     \
    return parsers::regex(re);                                                 \
  }())

#endif // PARSER_REGEX_H