#include "parsers.h"
#include "gtest/gtest.h"
#include <vector>

TEST(ParserTest, Choice) {
  auto parser = parsers::choice(
      parsers::digits, parsers::map(parsers::string("null"),
                                    [](auto s) -> int { return s.size(); }));

  auto parsed1 = parser("11* 456 ").value();
  EXPECT_EQ(parsed1.value, 11);
  EXPECT_EQ(parsed1.remaining, "* 456 ");

  auto parsed2 = parser("null * 123").value();
  EXPECT_EQ(parsed2.value, 4);
  EXPECT_EQ(parsed2.remaining, " * 123");

  auto parsed3 = parser("fuga * 123");
  EXPECT_TRUE(!parsed3.has_value());
}