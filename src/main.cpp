#include "parsers.h"
#include "json/array.h"
#include "json/value.h"
#include <iostream>

int main() {
  auto parsed2 = std::move(json::array(R"([ ])").value());
  std::cout << parsed2.value.original() << std::endl;
  return 0;
}