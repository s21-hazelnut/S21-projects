#ifndef SRC_S21_SMARTCALC_H_
#define SRC_S21_SMARTCALC_H_

#include <cmath>
#include <cstring>
#include <string>

namespace s21 {
class back {
 public:
  typedef struct elem {
    double value;
    int oper;
  } elem;
  elem parced_stack[256];
  elem polish_stack[256];
  std::string S_input;
  back();

  back(std::string S_inp);
  // back(const back&);
  // back(back&&);
  // back& operator=(const back&);
  // back& operator=(back&&);
  ~back();

  int Checks();
  double GetCalcRezult(double x);
  void Credit(double summ, int term, double proc, int type, double *month,
              double *over, double *all);

  // private: /////////////////
  /* Значенеие oper:
  0 - начало стека;
  1 - число - нет оператора;
  2 - открывающая скобка;
  3 - закрывающая скобка;
  4 - сложение;
  5 - вычетание;
  6 - умножение;
  7 - деление;
  8 - возведение в степень;
  9 - остаток от деления mod;
  10 - sin;
  11 - cos;
  12 - tan;
  13 - acos;
  14 - asin;
  15 - atan;
  16 - sqrt;
  17 - ln;
  18 - log;
  19 - X, переменная;
  20 - резерв (унарный плюс);
  21 - резерв (унарный минус);
  22 - конец стека;
  */
  void Regist(std::string *S_str);
  void SetElemNull(elem *el);
  void DelSpace(std::string *S_str);
  int GetParcedStack();
  int pop(int *oper, double *value, elem *stack);
  int push(const int *oper, const double *value, elem *stack);
  int peek(int *oper, double *value, elem *stack);
  int OperPriority(elem *nit);
  int GetStackEnd(elem *unit);
  void InverseStack(elem *parced_stack);
  int GetPolishStack();
  int IsXInInput();
  double Calc(double x);
};
}  // namespace s21
#endif  // SRC_S21_SMARTCALC_H_
