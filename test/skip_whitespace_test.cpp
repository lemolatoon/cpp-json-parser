#include "parser.h"
#include "parser/digits.h"
#include "parser/skip_whitespace.h"
#include "gtest/gtest.h"

TEST(ParserTest, SkipWhitespace) {
  auto parser = parsers::skip_whitespace<int>(parsers::digits);
  auto parsed1 = parser(" \t\n  123* 456 ").value();
  EXPECT_EQ(parsed1.value, 123);
  EXPECT_EQ(parsed1.remaining, "* 456 ");
  auto parsed2 = parsers::digits("fuga * 123");
  EXPECT_TRUE(!parsed2.has_value());
}