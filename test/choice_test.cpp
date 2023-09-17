#include "parser/character.h"
#include "parser/skip_whitespace.h"
#include "parsers.h"
#include "gtest/gtest.h"
#include <random>
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

TEST(ParserTest, Choices) {
  using namespace parsers;
  enum Kind {
    Digit,
    Null,
    A,
    B,
    Open,
    Close,
  };
  auto digitp = map(digits, [](auto d) { return Kind::Digit; });
  auto nullp = map(string("null"), [](auto s) { return Kind::Null; });
  auto ap = map(character('a'), [](auto c) { return Kind::A; });
  auto bp = map(character('b'), [](auto c) { return Kind::B; });
  auto openp = map(choices(character('{'), character('('), character('[')),
                   [](auto c) { return Kind::Open; });
  auto closep = map(choices(character('}'), character(')'), character(']')),
                    [](auto c) { return Kind::Close; });

  auto parser =
      many(skip_whitespace(choices(digitp, nullp, ap, bp, openp, closep)));

  auto parsed1 = parser("11 (456)null ").value();
  EXPECT_EQ(parsed1.value, (std::vector{Kind::Digit, Kind::Open, Kind::Digit,
                                        Kind::Close, Kind::Null}));
  EXPECT_EQ(parsed1.remaining, " ");

  auto parsed2 = parser("null {*} 123").value();
  EXPECT_EQ(parsed2.value, (std::vector{Kind::Null, Kind::Open}));
  EXPECT_EQ(parsed2.remaining, "*} 123");

  auto parsed3 = parser("11 ([{]})Null ").value();
  EXPECT_EQ(parsed3.value, (std::vector{
                               Kind::Digit,
                               Kind::Open,
                               Kind::Open,
                               Kind::Open,
                               Kind::Close,
                               Kind::Close,
                               Kind::Close,
                           }));
  EXPECT_EQ(parsed3.remaining, "Null ");

  auto parsed4 = parser(" fuga * 123").value();
  EXPECT_EQ(parsed4.value, (std::vector<Kind>{}));
  EXPECT_EQ(parsed4.remaining, " fuga * 123");
}