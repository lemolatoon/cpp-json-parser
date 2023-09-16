#include "parser.h"
#include "parser/choice.h"
#include "parser/digits.h"
#include "parser/map.h"
#include "parser/string.h"
#include "gtest/gtest.h"
#include <vector>

TEST(ParserTest, Choice) {
  auto parser = parsers::choice<int>(
      parsers::digits,
      parsers::map<std::string_view, int>(
          parsers::string("null"), [](auto s) -> int { return s.size(); }));

  auto parsed1 = parser("11* 456 ").value();
  EXPECT_EQ(parsed1.value, 11);
  EXPECT_EQ(parsed1.remaining, "* 456 ");

  auto parsed2 = parsers::digits("null * 123");
  EXPECT_EQ(parsed1.value, 4);
  EXPECT_EQ(parsed1.remaining, "* 123");

  auto parsed3 = parsers::digits("fuga * 123");
  EXPECT_TRUE(!parsed2.has_value());
}