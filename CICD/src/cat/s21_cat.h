#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#include <getopt.h>
#include <stdarg.h>
#include <stdio.h>

struct Flags {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
  int E;
  int T;
  int error;
} flag;
int my_optind = 0;

struct Flags parce_in(int argc, char *argv[], struct Flags flag);
int process_symbol(unsigned char symbol);

#endif  // SRC_CAT_S21_CAT_H_
