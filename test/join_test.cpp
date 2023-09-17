#include "parser/digits.h"
#include "parser/skip_whitespace.h"
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

TEST(ParserTest, Joins) {
  auto plus_minus =
      parsers::choice<char>(parsers::character('+'), parsers::character('-'));

  auto parser = parsers::joins(
      parsers::skip_whitespace<char>(plus_minus), parsers::digits,
      parsers::skip_whitespace<std::string_view>(parsers::string("abc")),
      parsers::skip_whitespace<int>(parsers::digits));

  auto parsed1 = parser("+11 abc 456 ").value();
  EXPECT_EQ(parsed1.value, (std::tuple<char, int, std::string_view, int>{
                               '+', 11, "abc", 456}));
  EXPECT_EQ(parsed1.remaining, " ");

  auto parsed2 = parser("-11 abc 256 ").value();
  EXPECT_EQ(parsed2.value, (std::tuple<char, int, std::string_view, int>{
                               '-', 11, "abc", 256}));
  EXPECT_EQ(parsed2.remaining, " ");

  auto parsed3 = parser("-fuga* 456 ");
  EXPECT_TRUE(!parsed3.has_value());

  auto parsed4 = parser("-11 ABC 256 ");
  EXPECT_TRUE(!parsed4.has_value());

  auto parser2 = parsers::choice<std::string>(
      map<std::string_view, std::string>(parsers::string("[error]"),
                                         [](auto s) { return std::string(s); }),
      map<std::tuple<char, int, int>, std::string>(
          parsers::joins(parsers::skip_whitespace<char>(plus_minus),
                         parsers::digits,
                         parsers::skip_whitespace<int>(parsers::digits)),
          [](auto tuple) -> std::string {
            auto [sign, digit, digit2] = tuple;
            if (sign == '-') {
              digit *= -1;
            }
            return std::to_string(digit - digit2);
          }));

  auto parsed2_1 = parser2("-11 23 11").value();
  EXPECT_EQ(parsed2_1.value, "-34");
  EXPECT_EQ(parsed2_1.remaining, " 11");

  auto parsed2_2 = parser2("[error] 11").value();
  EXPECT_EQ(parsed2_2.value, "[error]");
  EXPECT_EQ(parsed2_2.remaining, " 11");

  auto parsed2_3 = parser2("11 [error] 11");
  EXPECT_TRUE(!parsed2_3.has_value());
}