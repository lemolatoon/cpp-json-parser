#ifndef PARSER_SKIP_WHITESPACE_H
#define PARSER_SKIP_WHITESPACE_H
#include "parser.h"
#include <optional>

namespace parsers {

/**
 *  パーサーを受けとり、最初に空白を読み飛ばすてから、パーサーを適用するようなパーサーを返す。
 *  具体的には、' ', '\t', '\n' を読み飛ばす。
 */
auto inline skip_whitespace(Parser auto parser)
    -> Parser<ParserReturnType<decltype(parser)>> auto{
  return
      [=](std::string_view input)
          -> std::optional<ParserResult<ParserReturnType<decltype(parser)>>> {
        if (input.empty())
          return parser(input);

        int cursor = 0;
        for (char ch : input) {
          if (ch == ' ' || ch == '\t' || ch == '\n') {
            cursor++;
            continue;
          }
          break;
        }
        // 0 <= cursor <= input.size() になっている。
        return parser(input.substr(cursor));
      };
}

} // namespace parsers

#endif // PARSER_SKIP_WHITESPACE_H