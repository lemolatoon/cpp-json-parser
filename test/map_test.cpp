#include "parser.h"
#include "parser/digits.h"
#include "parser/map.h"
#include "gtest/gtest.h"

TEST(ParserTest, Map) {
  auto mapper = [](int x) { return x + 1; };
  auto parser = parsers::map<int, int>(parsers::digits, mapper);

  auto parsed1 = parser("123* 456 ").value();
  EXPECT_EQ(parsed1.value, 124);
  EXPECT_EQ(parsed1.remaining, "* 456 ");

  auto parsed2 = parsers::digits("fuga * 123");
  EXPECT_TRUE(!parsed2.has_value());
}