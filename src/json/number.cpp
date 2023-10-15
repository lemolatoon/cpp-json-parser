#include "json/number.h"
#include "parser/character.h"
#include "parsers.h"
#include "json/types.h"

#include <tuple>

using namespace parsers;

namespace json {

/*
 * number
 * integer fraction exponent
 */
std::optional<ParserResult<Number>> number(std::string_view input) {
  // clang-format off
  return map(joins(
      integer,
      fraction,
      exponent
  ), [](auto tuple) {
      auto [integer, fraction, exponent] = tuple;
      return Number(integer, fraction, exponent);
  })(input);
  // clang-format on
}

/*
 * integer
 * digit
 * onenine digits
 * '-' digit
 * '-' onenine digits
 */
std::optional<ParserResult<std::string>> integer(std::string_view input) {
  auto mapper = [](auto tuple) {
    auto [first, second] = tuple;
    return std::string{first} + std::string{second};
  };
  // clang-format off
  return choices(
          map(join(
              onenine,
              digits
          ), mapper),
          map(digit, [](auto ch) {return std::string{ch};}),
          map(join(
              character('-'),
              digit
          ), mapper),
          map(join(
              character('-'),
              map(join(
                  onenine,
                  digits
              ), mapper)
          ), mapper)
  )(input);
  // clang-format on
}

/*
 * digits
 * digit
 * digit digits
 */
std::optional<ParserResult<std::string>> digits(std::string_view input) {
  // clang-format off
  return choice(
      map(join(
          digit,
          digits
      ), [](auto tuple) {
          auto [first, second] = tuple;
          return first + second;
      }),
      map(digit, [](auto ch) {return std::string{ch};})
  )(input);
  // clang-format on
}

/*
 * digit
 * '0'
 * onenine
 */
std::optional<ParserResult<char>> digit(std::string_view input) {
  // clang-format off
  return choice(
      character('0'),
      onenine
  )(input);
  // clang-format on
}

/*
 * onenine
 * '1' . '9'
 */
std::optional<ParserResult<char>> onenine(std::string_view input) {
  // clang-format off
  return choices(
      character('1'),
      character('2'),
      character('3'),
      character('4'),
      character('5'),
      character('6'),
      character('7'),
      character('8'),
      character('9')
  )(input);
  // clang-format on
}

/*
 * fraction
 * ""
 * '.' digits
 */
std::optional<ParserResult<std::optional<std::string>>>
fraction(std::string_view input) {
  // clang-format off
  return choices(
      map(join(
          character('.'),
          digits
      ), [](auto tuple) -> std::optional<std::string> {
          auto [first, second] = tuple;
          return std::string{second};
      }),
      map(string(""), [](auto view) -> std::optional<std::string> { return std::nullopt; })
  )(input);
  // clang-format on
}

/*
 * exponent
 * ""
 * 'E' sign digits
 * 'e' sign digits
 */
std::optional<ParserResult<std::optional<std::pair<char, std::string>>>>
exponent(std::string_view input) {
  // clang-format off
  return choice(
    map(joins(
        choices(
            character('E'),
            character('e')
        ),
        sign,
        digits
    ), [](auto tuple) -> std::optional<std::pair<char, std::string>> {
        auto [first, second, third] = tuple;
        return std::make_pair(first, second + third);
    }),
    map(string(""), [](auto s) -> std::optional<std::pair<char, std::string>> {return std::nullopt;})
  )(input);
  // clang-format on
}

/*
 * sign
 * ""
 * '+'
 * '-'
 */
std::optional<ParserResult<std::string>> sign(std::string_view input) {
  auto to_string_mapper = [](auto view) { return std::string{view}; };
  // clang-format off
  return map(choices(
      string("+"),
      string("-"),
      string("")
  ), to_string_mapper)(input);
  // clang-format on
}

} // namespace json