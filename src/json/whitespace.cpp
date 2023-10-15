#include "parser.h"
#include "parsers.h"
#include "json/types.h"
#include <cassert>
#include <memory>
using namespace parsers;

namespace json {

static std::optional<ParserResult<Whitespace>> empty(std::string_view input) {
  std::string_view sv{""};
  return ParserResult<Whitespace>{.value = Whitespace{sv, sv},
                                  .remaining = input};
}

/*
 * ws
 * ""
 * '0020' ws
 * '000A' ws
 * '000D' ws
 * '0009' ws
 */
std::optional<ParserResult<Whitespace>> whitespace(std::string_view input) {
  // clang-format off
   
  return choices(
    map(
      join(
        choices(
          character(' '), 
          character('\t'), 
          character('\n'),
          character('\r')
        ),
        whitespace
      ),
      [&](auto ch) {
        auto [got, ws] = ch;
        std::string_view original = ws.original();
        auto new_original = std::string_view{input.data(), original.size() + 1};
        return Whitespace(new_original, new_original);
      }
    ),
    empty
  )(input);
  // clang-format on
}

} // namespace json