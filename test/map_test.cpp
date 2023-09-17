#include "parsers.h"
#include "gtest/gtest.h"
#include <vector>

TEST(ParserTest, Map) {
  auto mapper = [](auto x) { return std::vector(x / 2, x * 2); };
  auto parser = parsers::map(parsers::digits, mapper);

  auto parsed1 = parser("11* 456 ").value();
  EXPECT_EQ(parsed1.value, (std::vector{22, 22, 22, 22, 22}));
  EXPECT_EQ(parsed1.remaining, "* 456 ");

  auto parsed2 = parsers::digits("fuga * 123");
  EXPECT_TRUE(!parsed2.has_value());
}