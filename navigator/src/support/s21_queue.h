#ifndef SRC_SUPPORT_S21_QUEUE_H_
#define SRC_SUPPORT_S21_QUEUE_H_
#include <initializer_list>
#include <iostream>

#include "s21_list.h"
namespace s21 {

template <typename T>
class queue : public list<T> {
 public:
  using const_reference = const T &;

  queue() {}
  explicit queue(std::initializer_list<T> const &items) : list<T>(items) {}
  void push(const_reference value) { this->push_front(value); }
  T pop() {
    T res = this->back();
    this->pop_back();
    return res;
  }
};
};  // namespace s21

#endif  // SRC_SUPPORT_S21_QUEUE_H_