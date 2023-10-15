#include "json/number.h"
#include "parser/character.h"
#include "parsers.h"
#include "json/types.h"

#include <string_view>
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
std::optional<ParserResult<std::string_view>> integer(std::string_view input) {
  auto mapper = [&](auto tuple) {
    auto [got, view] = tuple;
    return std::string_view{input.data(), view.size() + 1};
  };
  // clang-format off
  auto onenine_digits = map(
      join(
        onenine,
        digits
      ), 
      mapper
    );
  return choices(
    onenine_digits,
    map(
      digit, 
      [&](auto ch) {return std::string_view{input.data(), 1};}
    ),
    map(
      join(
        character('-'),
        digit
      ), 
      [&](auto tuple) {
        return std::string_view{input.data(), 2};
      }
    ),
    map(
      join(
        character('-'),
        onenine_digits
      ), 
      mapper
    )
  )(input);
  // clang-format on
}

/*
 * digits
 * digit
 * digit digits
 */
std::optional<ParserResult<std::string_view>> digits(std::string_view input) {
  // clang-format off
  return choice(
      map(
        join(
          digit,
          digits
        ), 
        [&](auto tuple) {
          auto [got, original] = tuple;
          return std::string_view{input.data(), original.size() + 1};
        }
      ),
    map(
      digit, 
      [&](auto ch) {
        return input.substr(0, 1);
      }
    )
  )(input);
  // clang-format on
} // namespace json

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
std::optional<ParserResult<std::optional<std::string_view>>>
fraction(std::string_view input) {
  // clang-format off
  return choices(
    map(join(
        character('.'),
        digits
    ), [&](auto tuple) -> std::optional<std::string_view> {
        auto [first, second] = tuple;
        return std::string_view{input.data(), second.size() + 1};
    }),
    map(
       empty, 
       [](auto view) -> std::optional<std::string_view> { return std::nullopt; }
    )
  )(input);
  // clang-format on
}

/*
 * exponent
 * ""
 * 'E' sign digits
 * 'e' sign digits
 */
std::optional<ParserResult<std::optional<std::string_view>>>
exponent(std::string_view input) {
  auto mapper = [&](auto tuple) -> std::optional<std::string_view> {
    auto [first, second, third] = tuple;
    return std::string_view{input.data(), 1 + second.size() + third.size()};
  };
  // clang-format off
  return choices(
    map(
      joins(
        choices(
          character('E'),
          character('e')
        ),
        sign,
        digits
      ),
      mapper
    ),
    map(
      empty,
      [](auto view) -> std::optional<std::string_view> { return std::nullopt; }
    )
  )(input);
  // clang-format on
} // namespace json

/*
 * sign
 * ""
 * '+'
 * '-'
 */
std::optional<ParserResult<std::string_view>> sign(std::string_view input) {
  // clang-format off
  return choices(
      map(
          choices(
          string("+"),
          string("-")
      ),
      [&](auto ch) {
          return std::string_view{input.data(), 1};
      }
      ),
      empty
  )(input);
  // clang-format on
}

} // namespace json