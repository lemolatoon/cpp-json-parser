#include "parsers.h"
#include "json/json.h"
#include <iostream>

int main() {
  auto parsed1 = std::move(
      json::parse(R"({"a": true, "b": false, "c": null, "あ": "abc" })")
          .value());
  std::cout << (*parsed1)["a"]->original() << std::endl;
  std::cout << (*parsed1)["あ"]->original() << std::endl;

  auto parsed2 = std::move(
      json::parse(
          R"({"a": [true, false], "b": false, "c": null, "あ": "abc" })")
          .value());
  std::cout << (*(*parsed2)["a"])[0]->original() << std::endl;

  auto parsed3 = std::move(json::parse(R"([1, 2, 3])").value());
  std::cout << (*parsed3)[0]->original() << std::endl;
  return 0;
}