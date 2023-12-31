#include "gtest/gtest.h"

#include "json/object.h"
#include "json/types.h"

TEST(JSON_PARSER, OBJECT) {
  using namespace json;
  auto parsed1 = std::move(
      object(R"({"a": true, "b": false, "c": null, "あ": "abc" })").value());
  EXPECT_EQ(parsed1.value.original(),
            R"({"a": true, "b": false, "c": null, "あ": "abc" })");
  EXPECT_EQ(parsed1.remaining, "");
  auto parsed1_iter = parsed1.value.value().begin();
  auto parsed1_iter_end = parsed1.value.value().end();
  EXPECT_EQ(parsed1_iter->first, icu::UnicodeString("a"));
  auto parsed1_1 =
      dynamic_cast<json::True *>(parsed1_iter->second.value().value.get());
  EXPECT_NE(parsed1_1, nullptr);
  EXPECT_EQ(parsed1_1->original(), "true");

  parsed1_iter++;
  EXPECT_EQ(parsed1_iter->first, icu::UnicodeString("b"));
  auto parsed1_2 =
      dynamic_cast<json::False *>(parsed1_iter->second.value().value.get());
  EXPECT_NE(parsed1_2, nullptr);
  EXPECT_EQ(parsed1_2->original(), "false");

  parsed1_iter++;
  EXPECT_EQ(parsed1_iter->first, icu::UnicodeString("c"));
  auto parsed1_3 =
      dynamic_cast<json::Null *>(parsed1_iter->second.value().value.get());
  EXPECT_NE(parsed1_3, nullptr);
  EXPECT_EQ(parsed1_3->original(), "null");

  parsed1_iter++;
  EXPECT_EQ(parsed1_iter->first, icu::UnicodeString("あ"));
  auto parsed1_4 =
      dynamic_cast<json::String *>(parsed1_iter->second.value().value.get());
  EXPECT_NE(parsed1_4, nullptr);
  EXPECT_EQ(parsed1_4->original(), "\"abc\"");
  EXPECT_EQ(parsed1_4->unicode_string(), icu::UnicodeString("abc"));
  parsed1_iter++;
  EXPECT_EQ(parsed1_iter, parsed1_iter_end);

  // auto parsed2 = std::move(object(R"({ })").value());
  // EXPECT_EQ(parsed2.value.original(), "{ }");
  // EXPECT_EQ(parsed2.value.value().size(), 0);
}
