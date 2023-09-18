#include "gtest/gtest.h"
#include "json/number.h"

TEST(JsonParser, Number) {
  using namespace json;
  auto parsed1 = number("123.").value();
  EXPECT_EQ(parsed1.value, Number{std::string{"123"}});
  EXPECT_EQ(parsed1.remaining, ".");

  auto parsed2 = number("123.456e").value();
  EXPECT_EQ(parsed2.value, Number{std::string{"123.456e"}});
  EXPECT_EQ(parsed2.remaining, "");

  auto parsed3 = number("123e-789.").value();
  EXPECT_EQ(parsed3.value, Number{std::string{"123e-789"}});
  EXPECT_EQ(parsed3.remaining, ".");

  auto parsed5 = number("123.456e+789").value();
  EXPECT_EQ(parsed5.value, Number{std::string{"123.456e+789"}});
  EXPECT_EQ(parsed5.remaining, "");

  auto parsed6 = number("e123.456e+789");
  EXPECT_TRUE(!parsed6.has_value());
}