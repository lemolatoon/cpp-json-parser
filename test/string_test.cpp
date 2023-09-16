#include "parsers.h"
#include "gtest/gtest.h"

TEST(ParserTest, String) {
  Parser<std::string_view> auto hello_parser = parsers::string("hello");
  auto parsed1 = hello_parser("hello world").value();
  EXPECT_EQ(parsed1.value, "hello");
  EXPECT_EQ(parsed1.remaining, " world");
  auto parsed2 = hello_parser("Hello parser");
  EXPECT_TRUE(!parsed2.has_value());
}