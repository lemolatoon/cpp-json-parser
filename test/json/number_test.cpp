#include "gtest/gtest.h"
#include "json/number.h"
#include <optional>

TEST(JsonParser, Number) {
  using namespace json;
  auto parsed1 = std::move(number("123.").value());
  auto expected_value1 = Number(std::string{"123"}, std::nullopt, std::nullopt);
  EXPECT_EQ(parsed1.value, expected_value1);
  EXPECT_EQ(parsed1.value.original(), "123");
  EXPECT_EQ(parsed1.value.value(), 123.);
  EXPECT_EQ(parsed1.remaining, ".");

  auto parsed2 = std::move(number("123.456e").value());
  auto expected_value2 =
      Number(std::string{"123.456"}, std::nullopt, std::nullopt);
  EXPECT_EQ(parsed2.value, expected_value2);
  EXPECT_EQ(parsed2.value.original(), "123.456");
  EXPECT_DOUBLE_EQ(parsed2.value.value(), static_cast<double>(123.456));
  EXPECT_EQ(parsed2.remaining, "e");

  auto parsed3 = std::move(number("123e-7.").value());
  EXPECT_EQ(parsed3.value.integral(), "123");
  EXPECT_FALSE(parsed3.value.fraction().has_value());
  EXPECT_EQ(parsed3.value.exponent(), "e-7");
  EXPECT_EQ(parsed3.value.original(), "123e-7");
  EXPECT_DOUBLE_EQ(parsed3.value.value(), static_cast<double>(123e-7));
  EXPECT_EQ(parsed3.remaining, ".");

  auto parsed5 = std::move(number("123.456E+5").value());
  EXPECT_EQ(parsed5.value.integral(), "123");
  EXPECT_EQ(parsed5.value.fraction(), ".456");
  EXPECT_EQ(parsed5.value.exponent(), "E+5");
  EXPECT_EQ(parsed5.value.original(), "123.456E+5");
  EXPECT_DOUBLE_EQ(parsed5.value.value(), static_cast<double>(123.456E+5));
  EXPECT_EQ(parsed5.remaining, "");

  auto parsed6 = std::move(number("e123.456e+789"));
  EXPECT_TRUE(!parsed6.has_value());
}