#include "parsers.h"
#include "gtest/gtest.h"

TEST(ParserTest, Separated) {
  auto parser = parsers::separated(parsers::digits, parsers::character(','));

  auto parsed1 = parser("12,123,456 ").value();
  EXPECT_EQ(parsed1.value, (std::vector{12, 123, 456}));
  EXPECT_EQ(parsed1.remaining, " ");

  auto parsed2 = parser("fuga,123").value();
  EXPECT_EQ(parsed2.value, (std::vector<int>{}));
  EXPECT_EQ(parsed2.remaining, "fuga,123");

  auto parsed3 = parser("12,123,456,");
  EXPECT_TRUE(!parsed3.has_value());
}