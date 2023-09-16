#include "parsers.h"
#include <iostream>

int main() {
  auto parser1 = parsers::string("string1");
  auto parser2 = parsers::string("string2");
  std::cout << parser1("string1").value().value;
  std::cout << parser2("string2").value().value;
  std::cout << "Hello World!\n";
  return 0;
}