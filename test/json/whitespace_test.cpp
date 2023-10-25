#include "gtest/gtest.h"
#include "json/whitespace.h"

TEST(JSON_PARSER, WHITESPACE) {
  using namespace json;
  auto parsed1 = whitespace("  \t\n\r").value();
  EXPECT_EQ(parsed1.value.original(), "  \t\n\r");
  EXPECT_EQ(parsed1.remaining, "");

  auto parsed2 = whitespace("  \t\n\rabc").value();
  EXPECT_EQ(parsed2.value.original(), "  \t\n\r");
  EXPECT_EQ(parsed2.remaining, "abc");

  auto parsed3 = whitespace("").value();
  EXPECT_EQ(parsed3.value.original(), "");
  EXPECT_EQ(parsed3.remaining, "");
}