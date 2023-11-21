#include "s21_SmartCalc.h"

using namespace s21;

back::back(std::string S_inp) { S_input = S_inp; }
back::back() {}
back::~back() {}

void back::Regist(std::string *S_str) {
  int n = 0;
  while (n <= (int)(S_str->size())) {
    if ((*S_str)[n] > 64 && (*S_str)[n] < 91) {
      (*S_str)[n] += 32;
    }
    n++;
  }
}

void back::DelSpace(std::string *S_str) {
  int n = 0;
  int m = 0;
  while (n <= (int)(S_str->size())) {
    //  замена запятой на точку
    if ((*S_str)[n] == ',') {
      (*S_str)[n] = '.';
    }
    //  замена : на /
    if ((*S_str)[n] == ':') {
      (*S_str)[n] = '/';
    }
    if ((*S_str)[n] == ' ') {
      n++;
    } else {
      (*S_str)[m] = (*S_str)[n];
      m++;
      n++;
    }
  }
  (*S_str)[m] = '\0';
}

int back::Checks() {
  Regist(&S_input);
  DelSpace(&S_input);
  int n = 0;
  int ret = 0;
  int dot = 0;
  int operat = 0;
  int error = 0;
  int skb = 0;
  int start = 1;
  int digit_dot = 0;
  // char *temp_input = input;
  while (S_input[n] != '\0') {
    // что нет непозволенных символов
    if (strchr(";<=>?@BFHJKPUVWYZ[\\]_`bfhjkpuvwyz{|}~", S_input[n])) {
      error = 1;
    }
    //  что скобки стоят правильно
    if (S_input[n] == '(') {
      skb++;
    }
    if (S_input[n] == ')') {
      skb--;
    }
    if (skb < 0) {
      error = 1;
    }
    // что нет двух точек в числе
    if (strchr("0123456789.,e", S_input[n]) && dot) {
      digit_dot = 1;
    }
    if (!strchr("0123456789.,", S_input[n])) {
      digit_dot = 0;
    }
    if (S_input[n] == '.' && digit_dot) {
      error = 1;
    }
    // что после точки идет дробная часть числа
    if (dot && !strchr("0123456789e", S_input[n])) {
      error = 1;
    }
    if (S_input[n] == '.') {
      dot = 1;
    } else {
      dot = 0;
    }
    // что нет двух операторов подряд
    if (operat && strchr("+-*/^", S_input[n])) {
      error = 1;
    }
    if (strchr("+-*/^", S_input[n])) {
      operat = 1;
    } else {
      operat = 0;
    }
    // что нет пропущенных символов в операторах
    if (strchr("odinqrg", S_input[n])) {
      error = 1;
    }
    // что нет неправильно написанных функций и перед ними опер или нач
    if (strchr("msctal", S_input[n])) {
      if (!start && !strchr("+-*/^(", S_input[n - 1]) &&
          !strchr("m", S_input[n])) {
        error = 1;
      }
      int flag = 1;
      if (!memcmp(&S_input[n], "mod", 3) || !memcmp(&S_input[n], "sin(", 4) ||
          !memcmp(&S_input[n], "cos(", 4) || !memcmp(&S_input[n], "tan(", 4) ||
          !memcmp(&S_input[n], "log(", 4)) {
        n += 2;
        flag = 0;
      }
      if (!memcmp(&S_input[n], "asin(", 5) ||
          !memcmp(&S_input[n], "acos(", 5) ||
          !memcmp(&S_input[n], "atan(", 5) ||
          !memcmp(&S_input[n], "sqrt(", 5)) {
        n += 3;
        flag = 0;
      }
      if (!memcmp(&S_input[n], "ln(", 3)) {
        n++;
        flag = 0;
      }
      if (flag) {
        error = 1;
      }
    }
    start = 0;
    n++;
  }
  if (skb || error) {
    ret = 1;
  }
  return ret;
}

int back::IsXInInput() {
  std::string temp = S_input;
  Regist(&temp);
  int flag = 0;
  int n = 0;
  while (temp[n] != '\0') {
    if (strchr("xX", temp[n])) {
      flag = 1;
    }
    n++;
  }
  return flag;
}

void back::SetElemNull(elem *el) {
  for (int i = 0; i < 255; i++) {
    el[i].oper = 0;
    el[i].value = 0.0;
  }
  el[1].oper = 22;
}

int back::GetParcedStack() {
  int parce_ret = 1;
  if (!Checks()) {
    // SetElemNull(parced_stack);
    int num = 1;
    int unarn = 0;
    int n = 0;
    std::string temp_input = S_input;
    while (temp_input[n] != '\0') {
      if (strchr("0123456789e.", temp_input[n]) ||
          (strchr("+-", temp_input[n]) && !unarn &&
           !strchr("msctal", temp_input[n + 1]))) {
        parced_stack[num].oper = 1;
        int sign = 1;
        if (temp_input[n] == '+') {
          n++;
        }
        if (temp_input[n] == '-') {
          sign = -1;
          n++;
        }
        double res = 0;
        int EX = 0;
        int point = 0;
        int ten = 1;
        while (strchr("0123456789.", temp_input[n]) && temp_input[n] != '\0') {
          if (temp_input[n] == '.') {
            point++;
            n++;
          }
          if (!point) {
            res = res * 10 + (temp_input[n] - 48);
            n++;
          } else {
            ten *= 10;
            res = res + (double)(temp_input[n] - 48) / ten;
            n++;
          }
        }
        if (temp_input[n] == 'e') {
          int sign_exp = 1;
          n++;

          if (temp_input[n] == '+') {
            n++;
          }
          if (temp_input[n] == '-') {
            sign_exp = -1;
            n++;
          }
          while (strchr("0123456789", temp_input[n]) && temp_input[n] != '\0') {
            EX = EX * 10 + (temp_input[n] - 48);
            n++;
          }
          EX = EX * sign_exp;
        }
        parced_stack[num].value = res * (EX ? pow(10, EX) : 1) * sign;
        num++;
      }
      if (temp_input[n] == '(') {
        parced_stack[num].oper = 2;
        num++;
        n++;
        unarn = 0;
      } else {
        unarn = 1;
      }
      if (temp_input[n] == ')') {
        parced_stack[num].oper = 3;
        num++;
        n++;
      }
      if (temp_input[n] == '+' && unarn) {
        parced_stack[num].oper = 4;
        num++;
        n++;
      }
      if (temp_input[n] == '-' && unarn) {
        parced_stack[num].oper = 5;
        num++;
        n++;
      }
      std::string tokens[] = {"is_null", "is_one",  "is_two", "is_three",
                              "is_four", "is_five", "*",      "/",
                              "^",       "mod",     "sin",    "cos",
                              "tan",     "acos",    "asin",   "atan",
                              "sqrt",    "ln",      "log",    "x"};
      for (int j = 6; j != 20; j++) {
        if (!memcmp(&temp_input[n], &tokens[j][0], tokens[j].size())) {
          parced_stack[num].oper = j;

          num++;
          n += tokens[j].size();
        }
      }
    }
    parced_stack[num].oper = 22;
    InverseStack(parced_stack);
  } else {
    parce_ret = 0;
  }
  return parce_ret;
}

int back::pop(int *oper, double *value, elem *stack) {
  int n = GetStackEnd(stack);
  if (n) {
    *oper = stack[n].oper;
    *value = stack[n].value;
    stack[n + 1].oper = 0;
    stack[n + 1].value = 0.0;
    stack[n].oper = 22;
    stack[n].value = 0.0;
  }
  return (n) ? 1 : 0;
}

int back::push(const int *oper, const double *value, elem *stack) {
  int n = GetStackEnd(stack);
  if (n < 254) {
    stack[n + 1].oper = *oper;
    stack[n + 1].value = *value;
    stack[n + 2].oper = 22;
    stack[n + 2].value = 0.0;
  }
  return (n < 256) ? 1 : 0;
}

int back::peek(int *oper, double *value, elem *stack) {
  int n = GetStackEnd(stack);
  if (n) {
    *oper = stack[n].oper;
    *value = stack[n].value;
  }
  return (n) ? 1 : 0;
}

// дает ноль при пустом стеке
int back::GetStackEnd(elem *unit) {
  int ret = 0;
  for (int i = 0; i < 255; i++) {
    if (unit[i].oper != 0 && unit[i].oper != 22) {
      ret = 300;
    }
  }
  if (ret) {
    int n = 255;
    while (unit[n].oper != 22 && n > 0) {
      n--;
    }
    ret = (n > 0) ? (n - 1) : 0;
  }
  return ret;
}

int back::OperPriority(elem *unit) {
  int n = GetStackEnd(unit);
  int ret;
  if (unit[n].oper == 1 || unit[n].oper == 19) {
    ret = 0;
  }
  if (unit[n].oper == 4 || unit[n].oper == 5) {
    ret = 1;
  }
  if (unit[n].oper == 6 || unit[n].oper == 7 || unit[n].oper == 9) {
    ret = 2;
  }
  if (unit[n].oper == 8 || unit[n].oper == 16) {
    ret = 3;
  }
  if (unit[n].oper == 17 || unit[n].oper == 18 ||
      (unit[n].oper >= 10 && unit[n].oper <= 15)) {
    ret = 4;
  }
  if (unit[n].oper == 2) {
    ret = 0;
  }
  if (unit[n].oper == 3) {
    ret = 0;
  }
  return ret;
}

void back::InverseStack(elem *parced_stack) {
  if (GetStackEnd(parced_stack)) {
    elem new_stack[256];
    SetElemNull(new_stack);
    int n = GetStackEnd(parced_stack);
    int m = 1;
    while (n) {
      new_stack[m].oper = parced_stack[n].oper;
      new_stack[m].value = parced_stack[n].value;
      n--;
      m++;
    }
    new_stack[m].oper = 22;
    new_stack[m].value = 0;
    for (int i = 0; i < 256; i++) {
      parced_stack[i].oper = new_stack[i].oper;
      parced_stack[i].value = new_stack[i].value;
    }
  }
}

int back::GetPolishStack() {
  elem temp_stack[256];
  SetElemNull(temp_stack);
  int o = 0;
  double v = 0;
  int temp_o = 0;
  double temp_v = 0;
  SetElemNull(temp_stack);
  while (GetStackEnd(parced_stack)) {
    peek(&o, &v, parced_stack);
    // если число
    if (o == 1 || o == 19) {
      pop(&temp_o, &temp_v, parced_stack);
      push(&temp_o, &temp_v, polish_stack);
    }
    // если префиксная функция
    if (o >= 10 && o <= 18) {
      pop(&temp_o, &temp_v, parced_stack);
      push(&temp_o, &temp_v, temp_stack);
    }
    // если открывающаяся скобка
    if (o == 2) {
      pop(&temp_o, &temp_v, parced_stack);
      push(&temp_o, &temp_v, temp_stack);
    }
    // если закрывающаяся скобка
    if (o == 3) {
      int temp_o1 = 0;
      double temp_v1 = 0;
      pop(&temp_o1, &temp_v1,
          parced_stack);  //  удаляем закрывающую скобка из входного стека
      while (GetStackEnd(temp_stack)) {
        pop(&temp_o1, &temp_v1, temp_stack);
        if (temp_o1 != 2) {
          push(&temp_o1, &temp_v1, polish_stack);
        } else {
          break;
        }
      }
      peek(&temp_o1, &temp_v1, temp_stack);
    }
    // если оператор
    if (o >= 4 && o <= 9) {
      peek(&temp_o, &temp_v, temp_stack);
      while (((temp_o >= 10 && temp_o <= 18) ||
              OperPriority(temp_stack) >= OperPriority(parced_stack)) &&
             GetStackEnd(temp_stack)) {
        int o3 = 0;
        double v3 = 0;
        pop(&o3, &v3, temp_stack);
        if (o3 != 2) push(&o3, &v3, polish_stack);
        peek(&temp_o, &temp_v, temp_stack);
      }
      pop(&temp_o, &temp_v, parced_stack);
      push(&temp_o, &temp_v, temp_stack);
    }
  }
  while (GetStackEnd(temp_stack)) {
    pop(&o, &v, temp_stack);
    push(&o, &v, polish_stack);
  }
  InverseStack(polish_stack);
  return 0;
}

double back::Calc(double x) {
  elem temp_stack[256];
  SetElemNull(temp_stack);
  int temp_o1 = 0;
  double temp_v1 = 0;
  int one = 1;
  while (GetStackEnd(polish_stack)) {
    peek(&temp_o1, &temp_v1, polish_stack);
    if (temp_o1 == 1) {
      push(&temp_o1, &temp_v1, temp_stack);
      pop(&temp_o1, &temp_v1, polish_stack);
    }
    if (temp_o1 == 19) {
      push(&one, &x, temp_stack);
      pop(&temp_o1, &temp_v1, polish_stack);
    }
    if (temp_o1 >= 10 && temp_o1 <= 18) {
      int temp_o3 = 0;
      double temp_v3 = 0;
      pop(&temp_o3, &temp_v3, temp_stack);
      pop(&temp_o1, &temp_v1, polish_stack);
      if (temp_o1 == 10) {
        double temp = sin(temp_v3);
        push(&one, &temp, temp_stack);
      }
      if (temp_o1 == 11) {
        double temp = cos(temp_v3);
        push(&one, &temp, temp_stack);
      }
      if (temp_o1 == 12) {
        double temp = tan(temp_v3);
        push(&one, &temp, temp_stack);
      }
      if (temp_o1 == 13) {
        double temp = acos(temp_v3);
        push(&one, &temp, temp_stack);
      }
      if (temp_o1 == 14) {
        double temp = asin(temp_v3);
        push(&one, &temp, temp_stack);
      }
      if (temp_o1 == 15) {
        double temp = atan(temp_v3);
        push(&one, &temp, temp_stack);
      }
      if (temp_o1 == 16) {
        double temp = sqrt(temp_v3);
        push(&one, &temp, temp_stack);
      }
      if (temp_o1 == 17) {
        double temp = log(temp_v3);
        push(&one, &temp, temp_stack);
      }
      if (temp_o1 == 18) {
        double temp = log10(temp_v3);
        push(&one, &temp, temp_stack);
      }
    }
    if (temp_o1 >= 4 && temp_o1 <= 9) {
      int temp_o3 = 0;
      double temp_v3 = 0;
      pop(&temp_o3, &temp_v3, temp_stack);
      int temp_o4 = 0;
      double temp_v4 = 0;
      pop(&temp_o4, &temp_v4, temp_stack);
      pop(&temp_o1, &temp_v1, polish_stack);
      if (temp_o1 == 4) {
        double temp = temp_v4 + temp_v3;
        push(&one, &temp, temp_stack);
      }
      if (temp_o1 == 5) {
        double temp = temp_v4 - temp_v3;
        push(&one, &temp, temp_stack);
      }

      if (temp_o1 == 6) {
        double temp = temp_v3 * temp_v4;
        push(&one, &temp, temp_stack);
      }
      if (temp_o1 == 7) {
        double temp = temp_v4 / temp_v3;
        push(&one, &temp, temp_stack);
      }
      if (temp_o1 == 8) {
        double temp = pow(temp_v4, temp_v3);
        push(&one, &temp, temp_stack);
      }
      if (temp_o1 == 9) {
        double temp = fmod(temp_v4, temp_v3);
        push(&one, &temp, temp_stack);
      }
    }
  }
  pop(&temp_o1, &temp_v1, temp_stack);
  return (!GetStackEnd(temp_stack)) ? temp_v1 : 0;
}

double back::GetCalcRezult(double x) {
  double rez = 0;
  if (!Checks()) {
    if (IsXInInput()) {
      GetParcedStack();
      GetPolishStack();
      rez = Calc(x);
    } else {
      GetParcedStack();
      GetPolishStack();
      rez = Calc(0.0);
    }

  } else {
    rez = NAN;
  }
  return rez;
}

void back::Credit(double summ, int term, double proc, int type, double *month,
                  double *over, double *all) {
  *month = *over = *all = NAN;
  if (summ && type && summ > 0 && (term > 0 && term < 1200) &&
      (proc > 0 && proc < 5000)) {
    // аннуитетеный
    if (type == 1) {
      *month =
          summ * (proc / 1200) / ((1 - pow((1 + proc / 1200), term * (-1))));
      *all = *month * term;
      *over = *all - summ;
    }
    // дифференцированый
    if (type == 2) {
      *month = summ * (proc / 1200) + summ / term;
      *all = 0;
      double part = summ / term;
      double summ_temp = summ;
      while (term) {
        *all += summ_temp * (proc / 1200) + summ_temp / term;
        summ_temp -= part;
        term--;
      }
      *over = *all - summ;
    }
  }
}
