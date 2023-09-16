#include "parsers.h"
#include "gtest/gtest.h"

TEST(ParserTest, Digits) {
  auto parsed1 = parsers::digits("123*456").value();
  EXPECT_EQ(parsed1.value, 123);
  EXPECT_EQ(parsed1.remaining, "*456");
  auto parsed2 = parsers::digits("fuga*123");
  EXPECT_TRUE(!parsed2.has_value());
}