#include "parsers.h"
using namespace parsers;

namespace json {

/*
 * ws
 * ""
 * '0020' ws
 * '000A' ws
 * '000D' ws
 * '0009' ws
 */
std::optional<ParserResult<Unit>> whitespace(std::string_view input) {
  return map(choices(map(string(""), [](auto v) { return ' '; }),
                     character(' '), character('\t'), character('\n'),
                     character('\r')),
             [](auto v) { return Unit{}; })(input);
}

} // namespace json