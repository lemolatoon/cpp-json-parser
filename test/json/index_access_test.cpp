#include "gtest/gtest.h"
#include "json/json.h"

TEST(JSON_VALUE, INDEX_OPERATOR) {
  auto parsed1 = std::move(
      json::parse(R"({"a": true, "b": false, "c": null, "あ": "abc" })")
          .value());
  EXPECT_NE(parsed1->as_ref()["a"]->as_true(), nullptr);
  EXPECT_NE(parsed1->as_ref()["b"]->as_false(), nullptr);
  EXPECT_NE(parsed1->as_ref()["c"]->as_null(), nullptr);
  EXPECT_EQ(parsed1->as_ref()["あ"]->as_string()->unicode_string(),
            icu::UnicodeString("abc"));

  auto parsed2 = std::move(json::parse(R"([0, 1, 2, 3, 4])").value());
  for (auto i : {0, 1, 2, 3, 4}) {
    EXPECT_NE(parsed2->as_ref()[i]->as_number(), nullptr);
    EXPECT_DOUBLE_EQ(parsed2->as_ref()[i]->as_number()->value(),
                     static_cast<double>(i));
  }

  auto json3 = R"(
{
  "name": "lemolablog",
  "version": "0.1.0",
  "private": true,
  "engines": {
    "node": "18.x"
  },
  "scripts": {
    "dev": "next dev",
    "build": "next build",
    "start": "next start",
    "lint:eslint": "next lint",
    "lint:eslint:fix": "yarn lint:eslint --fix",
    "lint:ci:eslint": "yarn lint:eslint --max-warnings 0",
    "lint:fmt": "yarn prettier './**/*.{js,jsx,ts,tsx,json,yml,css}'",
    "lint:fmt:fix": "yarn lint:fmt --write",
    "lint:ci:fmt": "yarn lint:fmt --check",
    "lint:ci": "yarn lint:ci:eslint && yarn lint:ci:fmt",
    "lint:fix": "yarn lint:eslint:fix && yarn lint:fmt:fix",
    "typecheck": "yarn tsc --noEmit",
    "check": "yarn lint:fmt:fix && yarn lint:ci && yarn run build",
    "storybook": "storybook dev -p 6006",
    "build-storybook": "storybook build"
  },
  "dependencies": {
    "@supabase/auth-helpers-nextjs": "^0.5.2",
    "@supabase/auth-helpers-react": "^0.3.1",
    "@supabase/auth-ui-react": "^0.2.6",
    "@supabase/supabase-js": "^2.1.3",
    "@vercel/og": "^0.0.21",
    "next": "13.0.6",
    "react": "18.2.0",
    "react-dom": "18.2.0",
    "react-icons": "^4.7.1",
    "styled-components": "^5.3.6",
    "typescript": "4.9.4",
    "uuid": "^9.0.0",
    "zenn-content-css": "^0.1.134",
    "zenn-embed-elements": "^0.1.134",
    "zenn-markdown-html": "^0.1.134"
  },
  "devDependencies": {
    "@storybook/addon-essentials": "^7.0.7",
    "@storybook/addon-interactions": "^7.0.7",
    "@storybook/addon-links": "^7.0.7",
    "@storybook/blocks": "^7.0.7",
    "@storybook/nextjs": "^7.0.7",
    "@storybook/react": "^7.0.7",
    "@storybook/testing-library": "^0.0.14-next.2",
    "@types/node": "18.11.11",
    "@types/react": "18.0.26",
    "@types/react-dom": "18.0.9",
    "@types/styled-components": "^5.1.26",
    "@types/uuid": "^9.0.0",
    "@typescript-eslint/eslint-plugin": "^5.45.1",
    "@typescript-eslint/parser": "^5.45.1",
    "eslint": "8.29.0",
    "eslint-config-next": "13.0.6",
    "eslint-plugin-react": "^7.31.11",
    "eslint-plugin-storybook": "^0.6.11",
    "prettier": "^2.8.1",
    "storybook": "^7.0.7"
  }
}
  )";

  auto parsed3 = std::move(json::parse(json3).value());
  EXPECT_EQ(parsed3->as_ref()["name"]->as_string()->unicode_string(),
            "lemolablog");
  EXPECT_EQ(parsed3->as_ref()["engines"]
                ->as_ref()["node"]
                ->as_string()
                ->unicode_string(),
            "18.x");
  EXPECT_EQ(parsed3->as_ref()["dependencies"]
                ->as_ref()["react"]
                ->as_string()
                ->unicode_string(),
            "18.2.0");
}