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

#define AS_TYPE(TO_TY)                                                         \
  if (auto p = dynamic_cast<TO_TY *>(this)) {                                  \
    return p;                                                                  \
  } else if (auto p = dynamic_cast<Value *>(this)) {                           \
    if (auto q = dynamic_cast<TO_TY *>(p->value().value.get())) {              \
      return q;                                                                \
    }                                                                          \
  }                                                                            \
  return nullptr;

Object *ValueBase::as_object(){AS_TYPE(Object)};
Array *ValueBase::as_array(){AS_TYPE(Array)};
String *ValueBase::as_string(){AS_TYPE(String)};
Number *ValueBase::as_number(){AS_TYPE(Number)};
True *ValueBase::as_true(){AS_TYPE(True)};
False *ValueBase::as_false(){AS_TYPE(False)};
Null *ValueBase::as_null(){AS_TYPE(Null)};

} // namespace json