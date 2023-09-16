#include <exception.h>
#include <string>

const char *ParserException::what() const noexcept { return message_.data(); }