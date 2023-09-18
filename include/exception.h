#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <string>

class ParserException : public std::exception {
private:
  const std::string message_;
  const std::string_view input_rest_;

public:
  ParserException(std::string message, std::string_view input_rest)
      : message_(message), input_rest_(input_rest){};
  const char *what() const noexcept override;
  const std::string error_message(std::string_view original_input) const;
};

#endif // EXCEPTION_H