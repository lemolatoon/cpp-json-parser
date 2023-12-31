#include "parsers.h"
#include "gtest/gtest.h"

TEST(ParserTest, Character) {
  Parser<char> auto h_char_parser = parsers::character('h');
  auto parsed1 = h_char_parser("hoge").value();
  EXPECT_EQ(parsed1.value, 'h');
  EXPECT_EQ(parsed1.remaining, "oge");
  auto parsed2 = h_char_parser("fuga");
  EXPECT_TRUE(!parsed2.has_value());
}