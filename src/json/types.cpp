#include "json/types.h"

namespace json {
ValueBase *ValueBase::operator[](icu::UnicodeString &key) {
  if (auto object = dynamic_cast<Object *>(this)) {
    return (*object)[key];
  }

  return nullptr;
}
ValueBase *ValueBase::operator[](size_t key) {
  if (auto array = dynamic_cast<Array *>(this)) {
    return (*array)[key];
  }

  return nullptr;
}
ValueBase *ValueBase::operator[](std::string_view key) {
  if (auto object = dynamic_cast<Object *>(this)) {
    return (*object)[key];
  }

  return nullptr;
}
} // namespace json