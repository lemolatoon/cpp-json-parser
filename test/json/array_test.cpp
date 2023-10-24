#include "gtest/gtest.h"
#include "json/array.h"
#include "json/types.h"
#include <unicode/unistr.h>
#include "parsers.h"

TEST(JSON_PARSER, ARRAY) {
  using namespace json;
  auto parsed1 = std::move(array(R"([true, false, null, "abc" ])").value());
  EXPECT_EQ(parsed1.value.original(), "[true, false, null, \"abc\" ]");
  auto parsed1_iter = parsed1.value.value().begin();
  auto parsed1_1 = dynamic_cast<json::True *>(parsed1_iter->value().value.get());
  EXPECT_NE(parsed1_1, nullptr);
  EXPECT_EQ(parsed1_1->original(), "true");
  // EXPECT_EQ(parsed1.remaining, ", false, null]");
  parsed1_iter++;
  auto parsed1_2 = dynamic_cast<json::False *>(parsed1_iter->value().value.get());
  EXPECT_NE(parsed1_2, nullptr);
  EXPECT_EQ(parsed1_2->original(), "false");

  parsed1_iter++;
  auto parsed1_3 = dynamic_cast<json::Null *>(parsed1_iter->value().value.get());
  EXPECT_NE(parsed1_3, nullptr);
  EXPECT_EQ(parsed1_3->original(), "null");

  parsed1_iter++;
  auto parsed1_4 = dynamic_cast<json::String *>(parsed1_iter->value().value.get());
  EXPECT_NE(parsed1_4, nullptr);
  EXPECT_EQ(parsed1_4->original(), "\"abc\"");
  EXPECT_EQ(parsed1_4->unicode_string(), icu::UnicodeString("abc"));
}