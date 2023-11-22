#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#include <getopt.h>
#include <regex.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

struct Flags {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  char *file_for_f;
  int o;
  int error;
} flag;
int my_optind = 0;
char pattern_f[10000];

struct Flags parce_in(int argc, char *argv[], struct Flags flag);
void print_line(struct Flags flag, int many_files, int NOprint,
                int last_line_is_empty, int line_num, FILE *file,
                char *dest_file_name, char *line, int comp);
void print_if_c_or_l(struct Flags flag, int many_files, int anti_line_sum,
                     int line_sum, char *dest_file_name);
char *get_line(FILE *file, char rezult[]);
char *get_f_pattern_from_file(struct Flags flag, char *pattern_f);
void file_elaboration(int my_optind, int argc, char *argv[], struct Flags flag);

#endif  // SRC_GREP_S21_GREP_H_
