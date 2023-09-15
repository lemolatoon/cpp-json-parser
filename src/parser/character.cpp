#include <parser.h>
#include <optional>

Parser<char> auto character(char c)
{
    return [=](std::string_view input) -> std::optional<ParserResult<char>>
    {
        if (input.empty())
            return std::nullopt;
        if (input.front() == c)
            return ParserResult<char>{c, input.substr(1)};
        return std::nullopt;
    };
}