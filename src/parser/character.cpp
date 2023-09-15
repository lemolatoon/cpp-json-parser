#include <parser.h>
#include <optional>

Parser<char> auto character(char c)
{
    return [=](std::string_view input)
    {
        // TODO
        return std::nullopt;
    };
}