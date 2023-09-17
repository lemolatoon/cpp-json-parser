#include "parsers.h"
#include "gtest/gtest.h"

TEST(ParserTest, Many) {
  auto parser = parsers::many(parsers::skip_whitespace(parsers::digits));

  auto parsed1 = parser(" 12  123* 456 ").value();
  EXPECT_EQ(parsed1.value, (std::vector{12, 123}));
  EXPECT_EQ(parsed1.remaining, "* 456 ");

  auto parsed2 = parser("fuga * 123").value();
  EXPECT_EQ(parsed2.value, (std::vector<int>{}));
  EXPECT_EQ(parsed2.remaining, "fuga * 123");
}