#ifndef JSON_TYPES_H
#define JSON_TYPES_H

#include "parser.h"
#include <exception>
#include <iostream>
#include <limits>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <unicode/unistr.h>
#include <variant>
#include <vector>

namespace json {

class ValueBase {
protected:
  std::string_view original_;

public:
  // make this class polymorphic
  virtual ~ValueBase() = default;
  // // we need constructor
  // ValueBase() = default;
  // // we need move constructor
  // ValueBase(ValueBase &&) = default;
  // ValueBase &operator=(ValueBase &&) = default;

  // ValueBase(const ValueBase &) = delete;
  // ValueBase &operator=(const ValueBase &) = delete;

  inline std::string_view original() const { return original_; }
};

class Whitespace {
private:
  std::string_view original_;

public:
  Whitespace(std::string_view original) : original_{original} {}

  inline std::string_view original() const { return original_; }
};

class Number : public json::ValueBase {

private:
  std::string_view integral_;
  std::optional<std::string_view> fraction_;
  std::optional<std::string_view> exponent_;
  double value_;

public:
  inline Number(std::string_view integral,
                std::optional<std::string_view> fraction,
                std::optional<std::string_view> exponent)
      : integral_{integral}, fraction_{fraction}, exponent_{exponent} {
    this->original_ = integral;
    if (fraction_.has_value()) {
      // fraction (including the dot `.`)
      this->original_ = std::string_view{
          this->original_.data(), this->original_.size() + (*fraction_).size()};
    }

    if (exponent_.has_value()) {
      // exponent (including the `e` or `E`)
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
  inline double value() const { return value_; }

  inline bool operator==(const Number &rhs) const {
    return value() == rhs.value();
  }
}; // namespace json

class String : public json::ValueBase {
private:
  icu::UnicodeString value_;

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

class Value;

class Array : public json::ValueBase {
private:
  std::vector<Value> value_;

public:
  Array(std::string_view original, std::vector<Value> value)
      : value_{std::move(value)} {
    this->original_ = original;
  }
  inline const std::vector<Value> &value() const { return this->value_; }
};

class True : public json::ValueBase {
public:
  True(std::string_view original) { this->original_ = original; }
};

class False : public json::ValueBase {
public:
  False(std::string_view original) { this->original_ = original; }
};

class Null : public json::ValueBase {
public:
  Null(std::string_view original) { this->original_ = original; }
};

struct ValueData {
  Whitespace ws_before;
  // String | Number | Object | Array | True | False | Null
  std::unique_ptr<ValueBase> value;
  Whitespace ws_after;
};

class Value : public json::ValueBase {
private:
  ValueData value_;

public:
  Value(std::unique_ptr<ValueBase> value, Whitespace ws_before,
        Whitespace ws_after)
      : value_{ValueData{std::move(ws_before), std::move(value),
                         std::move(ws_after)}} {
    auto value_size = this->value_.value->original().size();
    this->original_ = std::string_view{
        ws_before.original().data(),
        ws_before.original().size() + value_size + ws_after.original().size()};
  }
  inline const ValueData &value() const { return value_; }
};

} // namespace json

#endif // JSON_TYPES_H