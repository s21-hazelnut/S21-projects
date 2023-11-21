#ifndef SRC_SUPPORT_S21_STACK_H_
#define SRC_SUPPORT_S21_STACK_H_

#include <cstring>
#include <initializer_list>

#include "s21_list.h"

namespace s21 {
template <class T>
class stack : public list<T> {
 public:
  using const_reference = const T &;

  stack() {}
  explicit stack(std::initializer_list<T> const &items) : list<T>(items) {}
  const_reference top() { return this->back(); }
  void push(const_reference value) { this->push_back(value); }
  T pop() {
    T ret = this->back();
    this->pop_back();
    return ret;
  }
};
};  // namespace s21

#endif  // SRC_SUPPORT_S21_STACK_H_