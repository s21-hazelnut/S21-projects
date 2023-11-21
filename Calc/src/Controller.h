#ifndef SRC_CONTROLLER_H
#define SRC_CONTROLLER_H

#include "s21_SmartCalc.h"

namespace s21 {
class Controller {
 private:
  back model;

 public:
  Controller() {}
  Controller(std::string S_inp) { model.S_input = S_inp; }
  int Checks() { return model.Checks(); }
  double GetCalcRezult(double x) { return model.GetCalcRezult(x); }
  void Credit(double summ, int term, double proc, int type, double *month,
              double *over, double *all) {
    model.Credit(summ, term, proc, type, month, over, all);
  }
};
}  // namespace s21

#endif  // SRC_CONTROLLER_H
