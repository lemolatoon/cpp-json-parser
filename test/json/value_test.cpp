#include "gtest/gtest.h"
#include "json/types.h"
#include "json/value.h"

TEST(JSON_PARSER, VALUE) {
  auto parsed1 = json::value(" true .").value();
  EXPECT_EQ(parsed1.value.original(), " true ");
  auto inner1 = dynamic_cast<json::True *>(parsed1.value.value().value.get());
  EXPECT_NE(inner1, nullptr);
  EXPECT_EQ(parsed1.remaining, ".");

  auto parsed2 = json::value(" false .").value();
  EXPECT_EQ(parsed2.value.original(), " false ");
  auto inner2 = dynamic_cast<json::False *>(parsed2.value.value().value.get());
  EXPECT_NE(inner2, nullptr);
  EXPECT_EQ(parsed2.remaining, ".");

  auto parsed3 = json::value(" null .").value();
  EXPECT_EQ(parsed3.value.original(), " null ");
  auto inner3 = dynamic_cast<json::Null *>(parsed3.value.value().value.get());
  EXPECT_NE(inner3, nullptr);
  EXPECT_EQ(parsed3.remaining, ".");

  auto parsed4 = json::value(" \"こんにちは！　世界！！\"  .").value();
  EXPECT_EQ(parsed4.value.original(), " \"こんにちは！　世界！！\"  ");
  auto inner4 = dynamic_cast<json::String *>(parsed4.value.value().value.get());
  EXPECT_NE(inner4, nullptr);
  EXPECT_EQ(inner4->unicode_string(),
            icu::UnicodeString::fromUTF8(u8"こんにちは！　世界！！"));

  EXPECT_EQ(parsed4.remaining, ".");

  auto parsed5 = json::value(" 125.51  .").value();
  EXPECT_EQ(parsed5.value.original(), " 125.51  ");
  auto inner5 = dynamic_cast<json::Number *>(parsed5.value.value().value.get());
  EXPECT_NE(inner5, nullptr);
  EXPECT_DOUBLE_EQ(inner5->value(), 125.51);

  EXPECT_EQ(parsed4.remaining, ".");
}