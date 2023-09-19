#include "gtest/gtest.h"
#include "json/number.h"
#include <optional>

TEST(JsonParser, Number) {
  using namespace json;
  auto parsed1 = number("123.").value();
  auto expected_value1 = Number(std::string{"123"}, std::nullopt, std::nullopt);
  EXPECT_EQ(parsed1.value, expected_value1);
  EXPECT_EQ(parsed1.remaining, ".");

  auto parsed2 = number("123.456e").value();
  auto expected_value2 = Number(std::string{"123"}, std::nullopt, std::nullopt);
  EXPECT_EQ(parsed2.value, expected_value2);
  EXPECT_EQ(parsed2.remaining, "e");

  auto parsed3 = number("123e-789.").value();
  EXPECT_EQ(parsed3.value,
            Number("123", std::nullopt,
                   std::make_optional(std::make_pair('e', "-789"))));
  EXPECT_EQ(parsed3.remaining, ".");

  auto parsed5 = number("123.456E+789").value();
  EXPECT_EQ(
      parsed5.value,
      Number("123", "456", std::make_optional(std::make_pair('E', "+789"))));
  EXPECT_EQ(parsed5.remaining, "");

  auto parsed6 = number("e123.456e+789");
  EXPECT_TRUE(!parsed6.has_value());
}