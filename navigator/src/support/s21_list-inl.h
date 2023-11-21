#include "s21_list.h"

namespace s21 {

template <typename T>
list<T>::list() : size_(0) {
  head_ = new element<T>(0);
  tail_ = head_;
}

template <typename T>
list<T>::list(size_type n) : list() {
  if (n > max_size()) {
    throw std::length_error("Not enough memory");
  }
  for (size_type i = 0; i < n; i++) {
    push_back(0);
  }
}

template <typename T>
list<T>::list(std::initializer_list<T> const &items) : list() {
  for (auto i = items.begin(); i != items.end(); ++i) {
    push_back(*i);
  }
}

template <typename T>
list<T>::list(const list<T> &l) : list() {
  element<T> *tmp = l.head_;
  while (tmp != nullptr) {
    push_back(tmp->data_);
    tmp = tmp->next_;
  }
}

template <typename T>
list<T>::list(list<T> &&l) : list() {
  *this = std::move(l);
}

template <typename T>
list<T>::~list() {
  clear();
  if (head_) delete head_;
}

template <typename T>
list<T> &list<T>::operator=(list<T> &&l) {
  std::swap(head_, l.head_);
  std::swap(tail_, l.tail_);
  std::swap(size_, l.size_);

  return *this;
}

template <typename T>
typename list<T>::const_reference list<T>::front() {
  if (empty()) throw std::length_error("the list is empty!");
  return head_->data_;
}

template <typename T>
typename list<T>::const_reference list<T>::back() {
  if (empty()) throw std::length_error("the list is empty!");
  return tail_->data_;
}

template <typename T>
typename list<T>::iterator list<T>::begin() {
  ListIterator ret(head_);
  return ret;
}

template <class T>
typename list<T>::ListIterator list<T>::ListIterator::operator++() {
  ptr_ = (ptr_) ? ptr_->next_ : nullptr;
  return *this;
}

template <class T>
typename list<T>::const_reference list<T>::ListConstIterator::operator*()
    const {
  return this->ptr_->data_;
}

template <class T>
typename list<T>::ListIterator list<T>::ListIterator::operator--() {
  ptr_ = (ptr_) ? ptr_->prev_ : nullptr;
  return *this;
}

template <class T>
bool list<T>::ListIterator::operator!=(iterator const &other) const {
  return (ptr_ != other.ptr_);
}

template <class T>
bool list<T>::ListIterator::operator==(iterator const &other) const {
  return (ptr_ == other.ptr_);
}

template <typename T>
typename list<T>::iterator list<T>::end() {
  ListIterator ret(tail_);
  return ret;
}

template <typename T>
bool list<T>::empty() {
  return (!size_) ? true : false;
}

template <typename T>
typename list<T>::size_type list<T>::size() {
  return size_;
}

template <typename T>
typename list<T>::size_type list<T>::max_size() {
  std::allocator<element<T>> get_MAX;
  return (list<T>::size_type)get_MAX.max_size();
}

template <typename T>
void list<T>::clear() {
  while (size_) pop_front();
}

template <typename T>
typename list<T>::iterator list<T>::insert(iterator pos,
                                           const_reference value) {
  if (pos.ptr_ == nullptr) {
    throw std::invalid_argument("Attempted a set to nullptr position");
  }
  if (pos.ptr_ == head_) {
    push_front(value);
  } else if (pos.ptr_ == tail_) {
    push_back(value);
  } else {
    element<T> *new_elem = new element<T>(value, pos.ptr_->prev_, pos.ptr_);
    pos.ptr_->prev_->next_ = new_elem;
    pos.ptr_->prev_ = new_elem;
    ++pos;
    ++size_;
  }
  return pos;
}

template <typename T>
void list<T>::erase(iterator pos) {
  if (pos.ptr_ == nullptr) throw std::length_error("the list is empty!");
  element<T> *tmp = pos.ptr_;
  if (pos.ptr_ == head_) {
    head_ = pos.ptr_->next_;
  } else if (pos.ptr_ == tail_) {
    element<T> *tmp1 = nullptr;
    tail_ = tail_->prev_;
    pos.ptr_->prev_->next_ = tmp1;
    if (tmp1) delete tmp1;
  } else {
    pos.ptr_->prev_->next_ = pos.ptr_->next_;
    pos.ptr_->next_->prev_ = pos.ptr_->prev_;
  }
  if (tmp) delete tmp;
  --size_;
}

template <typename T>
void list<T>::push_back(const_reference value) {
  if (empty()) {
    head_ = new element<T>(0);
    tail_ = head_;
    head_->data_ = value;
    ++size_;
  } else {
    element<T> *new_tail = new element<T>(value, tail_);
    tail_->next_ = new_tail;
    tail_ = new_tail;
    ++size_;
  }
}

template <typename T>
void list<T>::push_front(const_reference value) {
  if (empty()) {
    head_ = new element<T>(0);
    tail_ = head_;
    head_->data_ = value;
    ++size_;
  } else {
    element<T> *new_head = new element<T>(value, nullptr, head_);
    head_->prev_ = new_head;
    head_ = new_head;
    ++size_;
  }
}

template <typename T>
void list<T>::pop_back() {
  erase(end());
}

template <typename T>
void list<T>::pop_front() {
  erase(begin());
}

template <typename T>
void list<T>::swap(list &other) {
  std::swap(head_, other.head_);
  std::swap(tail_, other.tail_);
  std::swap(size_, other.size_);
}

template <typename T>
void list<T>::merge(list &other) {
  auto pos = other.begin();
  for (auto i = 0; i != other.size_; ++i) {
    int n = pos.ptr_->data_;
    push_front(n);
    ++pos;
  }
  sort();
}

template <typename T>
void list<T>::splice(const_iterator pos, list &other) {
  if (pos.ptr_ == nullptr)
    throw std::invalid_argument("Attempted a set to nullptr position");

  auto it = other.begin();
  for (auto n = 0; n < other.size_; n++) {
    insert(pos, it.ptr_->data_);
    ++it;
  }
  other.clear();
}

template <typename T>
void list<T>::reverse() {
  list<T> tmp;
  while (!empty()) {
    tmp.push_front(head_->data_);
    pop_front();
  }
  swap(tmp);
}

template <typename T>
void list<T>::unique() {
  if (empty()) throw std::invalid_argument("the list is empty!");
  auto pos = begin();
  auto pre_pos = pos;
  ++pos;
  while (pos != end()) {
    if (pos.ptr_->data_ == pre_pos.ptr_->data_) {
      erase(pos);
    } else {
      ++pre_pos;
    }
    ++pos;
  }
}

template <typename T>
void list<T>::sort() {
  if (empty()) throw std::invalid_argument("the list is empty!");
  auto pos = begin();
  auto m = 0;
  while (pos != end()) {
    auto pos1 = pos;
    ++pos1;

    auto n = size_ - m - 1;
    while (n) {
      if (pos1.ptr_->data_ < pos.ptr_->data_) {
        std::swap(pos.ptr_->data_, pos1.ptr_->data_);
      }
      ++pos1;
      n--;
    }
    ++pos;
    m++;
  }
}

}  // namespace s21
