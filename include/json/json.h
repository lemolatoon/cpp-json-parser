#ifndef JSON_JSON_H
#define JSON_JSON_H
#include "json/array.h"
#include "json/types.h"

namespace json {
inline std::optional<ParserResult<Value>> json(std::string_view input) {
  return json::element(input);
}

inline std::optional<std::unique_ptr<json::ValueBase>>
parse(std::string_view input) {
  auto json = json::json(input);
  return json.has_value()
             ? std::make_optional<std::unique_ptr<json::ValueBase>>(
                   std::move(json.value().value.value().value))
             : std::nullopt;
}
} // namespace json
#endif // JSON_JSON_H