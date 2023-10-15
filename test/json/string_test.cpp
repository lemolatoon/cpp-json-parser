#include "gtest/gtest.h"
#include "json/string.h"
#include <unicode/unistr.h>

TEST(JSON_PARSER, STRING) {
  using namespace json;
  auto parsed1 = string(R"("hello こんにちは"世界")").value();
  EXPECT_EQ(parsed1.value.original(), "\"hello こんにちは\"");
  EXPECT_EQ(parsed1.value.unicode_string(),
            icu::UnicodeString::fromUTF8(u8"hello こんにちは"));
  EXPECT_EQ(parsed1.remaining, "世界\"");
}