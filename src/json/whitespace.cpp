#include "parser.h"
#include "parsers.h"
#include "json/types.h"
#include <cassert>
#include <memory>
#include <string_view>
using namespace parsers;

namespace json {

static std::optional<ParserResult<Whitespace>> empty(std::string_view input) {
  std::cout << "empty_input:" << input << ":" << std::endl;
  std::string_view sv = input.substr(0, 0);
  return ParserResult<Whitespace>{.value = std::move(Whitespace{sv, sv}),
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
  std::cout << "whitespace_input:" << input << ":" << std::endl;
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
        auto [got, ws] = std::move(ch);
        std::string_view original = ws.original();
        auto new_original = std::string_view{input.data(), original.size() + 1};
        // std::cout << "got: " << (int)got << ":" << (int)' ' << std::endl;
        // std::cout << "original: " << original.size() << std::endl;
        // std::cout << "new_original.size(): " << new_original.size() << std::endl;
        // std::cout << "input: " << (void *)input.data() << std::endl;
        // std::cout << "new_original: " << (void *)new_original.data() << std::endl;
        return std::move(Whitespace(new_original, new_original));
      }
    ),
    empty
  )(input);
  // clang-format on
}

} // namespace json