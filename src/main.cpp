#include "parsers.h"
#include "json/json.h"
#include "json/types.h"
#include <iostream>

int main() {
  auto parsed1 = std::move(
      json::parse(R"({"a": true, "b": false, "c": null, "あ": "abc" })")
          .value());
  auto a = parsed1->as_ref()["a"];
  std::cout << "a is " << typeid(a).name() << std::endl;
  auto as_true = a->as_true();
  auto as_value = dynamic_cast<json::Value *>(a);
  std::cout << "as_true is " << typeid(a).name() << std::endl;
  std::cout << as_true << std::endl;
  std::cout << "as_value: " << as_value << std::endl;
  std::cout << (*parsed1)["あ"]->original() << std::endl;

  auto parsed2 = std::move(
      json::parse(
          R"({"a": [true, false], "b": false, "c": null, "あ": "abc" })")
          .value());
  auto got = parsed2->as_ref()["a"]->as_ref()[0];
  std::cout << got->original() << std::endl;

  auto parsed3 = std::move(json::parse(R"([1, 2, 3])").value());
  std::cout << parsed3->as_ref()[0]->original() << std::endl;
  return 0;
}