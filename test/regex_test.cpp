#include "parsers.h"
#include "gtest/gtest.h"

TEST(ParserTest, Regex) {
  Parser<std::string_view> auto parser = [] {
    static std::regex re(R"(^[a-zA-Z_][a-zA-Z0-9_]*)");
    return parsers::regex(re);
  }();
  auto parsed1 = parser("hoge").value();
  EXPECT_EQ(parsed1.value, "hoge");
  EXPECT_EQ(parsed1.remaining, "");

  auto parsed2 = parser("h_1&").value();
  EXPECT_EQ(parsed2.value, "h_1");
  EXPECT_EQ(parsed2.remaining, "&");

  auto parsed3 = parser("0__a");
  EXPECT_TRUE(!parsed3.has_value());

  // match intermediate
  Parser<std::string_view> auto parser2 = [] {
    static std::regex re(R"([a-zA-Z_][a-zA-Z0-9_]*)");
    return parsers::regex(re);
  }();

  auto parsed2_1 = parser2("hoge").value();
  EXPECT_EQ(parsed2_1.value, "hoge");
  EXPECT_EQ(parsed2_1.remaining, "");

  auto parsed2_2 = parser2("111 h_1&").value();
  EXPECT_EQ(parsed2_2.value, "h_1");
  EXPECT_EQ(parsed2_2.remaining, "&");

  auto parsed2_3 = parser2("0__a").value();
  EXPECT_EQ(parsed2_3.value, "__a");
  EXPECT_EQ(parsed2_3.remaining, "");
}

TEST(ParserTest, RegexMacro) {
  Parser<std::string_view> auto parser =
      REGEX(R"(^https://(aaa|bbb|ccc).example.jp)");
  auto parsed1 = parser("https://aaa.example.jp 123").value();
  EXPECT_EQ(parsed1.value, "https://aaa.example.jp");
  EXPECT_EQ(parsed1.remaining, " 123");

  auto parsed2 = parser("https://aaa.example.kamo.jp 123");
  EXPECT_TRUE(!parsed2.has_value());
}