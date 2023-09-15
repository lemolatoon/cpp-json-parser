#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <type_traits>
#include <optional>

template <class T>
struct ParserResult
{
    T value;
    std::string_view remaining;
};

template <class F, class T>
concept Parser = std::is_invocable_r_v<std::optional<ParserResult<T>>, F, std::string_view>;

#endif // PARSER_H