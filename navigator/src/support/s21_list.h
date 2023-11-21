#ifndef SRC_SUPPORT_S21_LIST_H_
#define SRC_SUPPORT_S21_LIST_H_

#include <cstring>
#include <initializer_list>
#include <iostream>
#include <limits>
#include <utility>

namespace s21 {

template <typename T>
struct element {
  element<T>() : element<T>({}) {}
  element<T>(T data, element<T> *prev = nullptr, element<T> *next = nullptr)
      : data_(data), next_(next), prev_(prev) {}

  T data_;            // содержимое элемента
  element<T> *next_;  // следующий элемент
  element<T> *prev_;  // предыдущий элемент
};

template <typename T>
class list {
 public:
  class ListIterator;
  class ListConstIterator;
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using iterator = ListIterator;
  using const_iterator = const ListIterator;
  using size_type = std::size_t;

  list();
  explicit list(size_type n);
  explicit list(std::initializer_list<value_type> const &items);
  list(const list &l);  // конструктор копирования
  list(list &&l);       // конструктор перемещения
  ~list();              // деструктор
  list<T> &operator=(list &&l);

  const_reference front();  // получить доступ к первому элементу
  const_reference back();  // получить доступ к последнему элементу

  iterator begin();  // возвращает итератор в начало
  iterator end();    // возвращает итератор в конец

  bool empty();      // проверяет, пуст ли контейнер
  size_type size();  // возвращает количество элементов
  size_type max_size();  // возвращает максимально возможное количество
                         // элементов

  void clear();  // очищает содержимое
  iterator insert(iterator pos, const_reference value);
  void erase(iterator pos);  // стирает элемент в позиции
  void push_back(const_reference value);  // добавляет элемент в конец
  void pop_back();  // удаляет последний элемент
  void push_front(const_reference value);  // добавляет элемент в голову
  void pop_front();        // удаляет первый элемент
  void swap(list &other);  // меняет содержимое
  void merge(list &other);  // объединяет два отсортированных списка
  void splice(const_iterator pos, list &other);
  void reverse();  // меняет порядок элементов
  void unique();  // удаляет последовательные повторяющиеся элементы
  void sort();    // сортирует элементы

  class ListIterator {
   public:
    element<T> *ptr_ = nullptr;
    explicit ListIterator(element<T> *init) : ptr_(init) {}
    ListIterator operator++();
    ListIterator operator--();
    T operator*();
    bool operator==(const iterator &other) const;
    bool operator!=(const iterator &other) const;
  };

  class ListConstIterator : public ListIterator {
   public:
    explicit ListConstIterator(element<T> *ptr = nullptr) : ListIterator(ptr) {}
    const_reference operator*() const;
  };

 private:
  element<T> *head_;
  element<T> *tail_;
  size_type size_;
};
}  // namespace s21

#include "s21_list-inl.h"

#endif  //  SRC_SUPPORT_S21_LIST_H_