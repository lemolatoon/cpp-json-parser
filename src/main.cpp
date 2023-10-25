#include "parsers.h"
#include "json/object.h"
#include "json/types.h"
#include <iostream>

int main() {
  auto parsed1 = std::move(
      json::object(R"({"a": true, "b": false, "c": null, "あ": "abc" })")
          .value());
  std::cout << parsed1.value.original() << std::endl;
  parsed1.value.value();
  return 0;
}