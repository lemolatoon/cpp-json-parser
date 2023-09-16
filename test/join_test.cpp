#include "parsers.h"
#include "gtest/gtest.h"

TEST(ParserTest, Join) {
  auto plus_minus =
      parsers::choice<char>(parsers::character('+'), parsers::character('-'));

  auto signed_digits_parser =
      parsers::join<char, int>(plus_minus, parsers::digits);

  auto parsed1 = signed_digits_parser("+11* 456 ").value();
  EXPECT_EQ(parsed1.value, (std::pair{'+', 11}));
  EXPECT_EQ(parsed1.remaining, "* 456 ");

  auto parsed2 = signed_digits_parser("-11* 456 ").value();
  EXPECT_EQ(parsed2.value, (std::pair{'-', 11}));
  EXPECT_EQ(parsed2.remaining, "* 456 ");

  auto parsed3 = signed_digits_parser("-fuga* 456 ");
  EXPECT_TRUE(!parsed3.has_value());

  auto parsed4 = signed_digits_parser("*11* 456 ");
  EXPECT_TRUE(!parsed3.has_value());
}