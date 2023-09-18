#ifndef JSON_TYPES_H
#define JSON_TYPES_H

#include <string>

struct Number {
  std::string inner;

  bool operator==(const Number &rhs) const { return inner == rhs.inner; }
};

#endif // JSON_TYPES_H