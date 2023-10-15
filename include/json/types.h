#ifndef JSON_TYPES_H
#define JSON_TYPES_H

#include <exception>
#include <iostream>
#include <limits>
#include <optional>
#include <string>
#include <tuple>
#include <unicode/unistr.h>

namespace json {

struct Number {

  inline Number(std::string integral, std::optional<std::string> fraction,
                std::optional<std::pair<char, std::string>> exponent)
      : integral_{integral}, fraction_{fraction}, exponent_{exponent} {
    original_cache_ = integral;
    if (fraction_.has_value()) {
      original_cache_ += "." + *fraction_;
    }

    if (exponent_.has_value()) {
      original_cache_ += std::get<0>(*exponent_) + std::get<1>(*exponent_);
    }

    try {
      value_cache_ = std::stod(original_cache_);
    } catch (std::exception &e) {
      std::cout << "Error: " << e.what() << " by " << original_cache_
                << std::endl;
      value_cache_ = std::numeric_limits<double>::max();
    }
  }

  inline double value() const { return value_cache_; }

  inline const std::string_view original() const { return original_cache_; }

  inline bool operator==(const Number &rhs) const {
    return value() == rhs.value();
  }

private:
  std::string integral_;
  std::optional<std::string> fraction_;
  std::optional<std::pair<char, std::string>> exponent_;
  std::string original_cache_;
  double value_cache_;
};

class String {
private:
  std::string original_;
  icu::UnicodeString unicode_string_;

public:
  String(std::string original, icu::UnicodeString unicode_string)
      : original_(original), unicode_string_(unicode_string) {}
  inline std::string_view original() const { return original_; }
  inline const icu::UnicodeString &unicode_string() const {
    return unicode_string_;
  }
};

} // namespace json

#endif // JSON_TYPES_H