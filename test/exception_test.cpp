#include "exception.h"
#include "gtest/gtest.h"

TEST(ExceptionTest, What) {
  auto exception = ParserException("message", "input_rest");
  std::string what = exception.what();
  std::string expected = "message";
  EXPECT_EQ(what, expected);
}

TEST(ExceptionTest, ErrorMessage) {
  std::string input = "startline\n012345678910\nendline\n";
  std::string_view input_view = input;
  std::string_view input_rest = input_view.substr(14);
  ASSERT_EQ(input_rest, "45678910\nendline\n");

  auto exception = ParserException("error_message", input_rest);
  EXPECT_EQ(exception.error_message(input_view),
            // clang-format off
R"(012345678910
    ^
    error_message)"
            // clang-format on
  );
}