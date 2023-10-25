#include "gtest/gtest.h"
#include "json/json.h"

#include <fstream>

TEST(JSON_PARSER, JSON) {
  // read from `vcpkg.json`
  std::ifstream vcpkg_json("./vcpkg.json");
  if (!vcpkg_json) {
    FAIL() << "Cannot open file `vcpkg.json`";
  }
  std::string content{std::istreambuf_iterator<char>(vcpkg_json),
                      std::istreambuf_iterator<char>()};
  vcpkg_json.close();

  auto parsed = json::parse(content).value();
  EXPECT_EQ(parsed->as_ref()["$schema"]->as_string()->unicode_string(),
            "https://raw.githubusercontent.com/microsoft/vcpkg-tool/main/docs/"
            "vcpkg.schema.json");
  auto deps = parsed->as_ref()["dependencies"]->as_array();
  EXPECT_EQ(deps->as_ref()[0]->as_ref()["name"]->as_string()->unicode_string(),
            "gtest");
  EXPECT_EQ(deps->as_ref()[1]->as_ref()["name"]->as_string()->unicode_string(),
            "icu");
}