#ifndef JSON_TYPES_H
#define JSON_TYPES_H

#include <optional>
#include <string>
#include <tuple>

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

    value_cache_ = std::stod(original_cache_);
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
  float value_cache_;
};

#endif // JSON_TYPES_H