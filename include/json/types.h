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

template <class T> class Base {
protected:
  std::string_view original_;
  T value_;

public:
  inline std::string_view original() const { return original_; }
  inline const T &value() const { return value_; }
};

class Whitespace : public json::Base<std::string_view> {
public:
  Whitespace(std::string_view original, std::string_view value) {
    this->original_ = original;
    this->value_ = value;
  }
};

class Number : public json::Base<double> {

private:
  std::string_view integral_;
  std::optional<std::string_view> fraction_;
  std::optional<std::string_view> exponent_;

public:
  inline Number(std::string_view integral,
                std::optional<std::string_view> fraction,
                std::optional<std::string_view> exponent)
      : integral_{integral}, fraction_{fraction}, exponent_{exponent} {
    this->original_ = integral;
    if (fraction_.has_value()) {
      // `.` + fraction
      this->original_ = std::string_view{
          this->original_.data(), this->original_.size() + (*fraction_).size()};
    }

    if (exponent_.has_value()) {
      // `e` or `E` + exponent
      this->original_ = std::string_view{
          this->original_.data(), this->original_.size() + (*exponent_).size()};
    }

    try {
      this->value_ = std::stod(std::string{this->original_});
    } catch (std::exception &e) {
      std::cout << "Error: " << e.what() << " by " << this->original_
                << std::endl;
      this->value_ = std::numeric_limits<double>::max();
    }
  }

  inline std::string_view integral() const { return integral_; }
  inline std::optional<std::string_view> fraction() const { return fraction_; }
  inline std::optional<std::string_view> exponent() const { return exponent_; }

  inline bool operator==(const Number &rhs) const {
    return value() == rhs.value();
  }
}; // namespace json

class String : public json::Base<icu::UnicodeString> {

public:
  String(std::string_view original, icu::UnicodeString unicode_string) {
    this->original_ = original;
    this->value_ = unicode_string;
  }
  inline std::string_view original() const { return original_; }
  inline const icu::UnicodeString &unicode_string() const {
    return this->value_;
  }
};

} // namespace json

#endif // JSON_TYPES_H