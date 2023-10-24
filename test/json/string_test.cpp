#include "gtest/gtest.h"
#include "json/string.h"
#include <unicode/unistr.h>

TEST(JSON_PARSER, STRING) {
  using namespace json;
  auto parsed1 = std::move(string(R"("hello こんにちは"世界")").value());
  EXPECT_EQ(parsed1.value.original(), "\"hello こんにちは\"");
  EXPECT_EQ(parsed1.value.unicode_string(),
            icu::UnicodeString::fromUTF8(u8"hello こんにちは"));
  EXPECT_EQ(parsed1.remaining, "世界\"");

  auto parsed2 = std::move(string(R"("\n")").value());
  EXPECT_EQ(icu::UnicodeString::fromUTF8(u8"\n")[0], (char16_t)'\n');
  EXPECT_EQ(parsed2.value.original(), "\"\\n\"");
  EXPECT_EQ(parsed2.value.unicode_string(),
            icu::UnicodeString::fromUTF8(u8"\n"));
  EXPECT_EQ(parsed2.remaining, "");

  auto parsed3 = std::move(string(R"("\u30aA")").value());
  EXPECT_EQ(parsed3.value.original(), std::string{"\"\\u30aA\""});
  EXPECT_EQ(parsed3.value.unicode_string(),
            icu::UnicodeString::fromUTF8(u8"オ"));
  EXPECT_EQ(parsed3.remaining, "");

  auto parsed4 = std::move(string(R"("abcdef!!)"));
  EXPECT_TRUE(!parsed4.has_value());

  auto parsed5 =
      std::move(string(R"("あ\\あ\"あ\/あ\bあ\fあ\nあ\rあ\tあ\u306cあ")").value());
  EXPECT_EQ(parsed5.value.original(),
            R"("あ\\あ\"あ\/あ\bあ\fあ\nあ\rあ\tあ\u306cあ")");
  // ぬ == U+306C
  EXPECT_EQ(
      parsed5.value.unicode_string(),
      icu::UnicodeString::fromUTF8(u8"あ\\あ\"あ/あ\bあ\fあ\nあ\rあ\tあぬあ"));
  EXPECT_EQ(parsed5.remaining, "");
}