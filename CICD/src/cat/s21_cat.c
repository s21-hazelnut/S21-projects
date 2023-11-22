#include "s21_cat.h"

struct Flags parce_in(int argc, char *argv[], struct Flags flag) {
  char *short_flags = "vbenstTE";
  struct option long_flags[] = {{"number-nonblank", 0, NULL, 'b'},
                                {"show-ends", 0, NULL, 'E'},
                                {"number", 0, NULL, 'n'},
                                {"squeeze-blank", 0, NULL, 's'},
                                {"show-tabs", 0, NULL, 'T'},
                                {"show-nonprinting", 0, NULL, 'v'},
                                {NULL, 0, NULL, 0}};
  int rezult;
  int opterr = 0;
  while ((rezult = getopt_long(argc, argv, short_flags, long_flags, &opterr)) !=
         -1) {
    switch (rezult) {
      case 'v': {
        flag.v = 1;
        break;
      };
      case 'b': {
        flag.b = 1;
        break;
      };
      case 'e': {
        flag.v = 1;
        flag.e = 1;
        break;
      };
      case 'n': {
        flag.n = 1;
        break;
      };
      case 's': {
        flag.s = 1;
        break;
      };
      case 't': {
        flag.v = 1;
        flag.t = 1;
        break;
      };
      case 'T': {
        flag.T = 1;
        break;
      };
      case 'E': {
        flag.E = 1;
        break;
      };
      case '?':
      default: {
        flag.error = 1;
        break;
      }
    }
  }
  my_optind = optind;
  return flag;
}

int process_symbol(unsigned char symbol) {
  int rez = 0;
  if (symbol == 127) {
    printf("^?");
    rez = 1;
  }
  if (symbol > 127 && symbol < 160 && !rez) {
    printf("M-");
    symbol = symbol - 128;
    rez = 1;
  }
  if (symbol < 32) {
    printf("^%c", (symbol + 64));
    rez = 1;
  }
  return rez;
}

void file_elaboration(int my_optind, int argc, char *argv[],
                      struct Flags flag) {
  for (int i = my_optind; i < argc; i++) {
    FILE *file;
    file = fopen(argv[i], "r");
    if (file) {
      int emptylinemeter = 1;
      int NOprint = 0;
      int line_start = 1;
      unsigned long line_num = 1;
      if (flag.b) line_num = 0;
      int start = 1;
      unsigned char old_symbol = '\n';
      while (!feof(file)) {
        unsigned char symbol;
        symbol = fgetc(file);  // get symbol from file
        int flag_convert = 0;
        if (flag.b && flag.n) flag.n = 0;
        if (flag.s) {
          if (symbol != '\n') emptylinemeter = 0;
          if (symbol == '\n') emptylinemeter++;
          if (emptylinemeter > 2) NOprint = 1;
        }
        if (symbol != '\n') start = 0;
        if (symbol > 127 && symbol < 160 && flag.v) {
          symbol = symbol - 128;
          flag_convert = 1;
        }
        if (flag.n && line_start && !feof(file) && !NOprint) {
          printf("%6lu\t", line_num);
          line_start = 0;
        }
        if (flag.n && symbol == '\n' && !NOprint) {
          line_num++;
          line_start = 1;
        }
        if (flag_convert) symbol = symbol + 128;
        if (flag.b && symbol != '\n' && old_symbol == '\n' && !start) {
          line_num++;
          line_start = 1;
        }
        if (flag.b && line_start && !start && !feof(file)) {
          printf("%6lu\t", line_num);
          line_start = 0;
        }
        if (((flag.e && flag.v && symbol == '\n') ||
             (flag.E && symbol == '\n')) &&
            !NOprint) {
          printf("$\n");
          NOprint = 1;
        }
        if ((flag.t && flag.v && symbol == '\t') ||
            (flag.T && symbol == '\t')) {
          printf("^I");
          NOprint = 1;
        }
        if (flag.v && symbol != '\t' && symbol != '\n' && !feof(file)) {
          if (process_symbol(symbol)) {
            NOprint = 1;
            if (symbol > 127 && symbol < 160) symbol = symbol - 128;
          }
        }
        old_symbol = symbol;
        if (flag.b && line_start && !start && !feof(file)) {
          printf("%6lu\t", line_num);
          line_start = 0;
        }
        if (feof(file)) NOprint = 1;
        if (!NOprint) printf("%c", symbol);
        NOprint = 0;
      }
      fclose(file);
    } else {
      printf("\ns21_cat: %s: No such file or directory\n", argv[i]);
    }
  }
}

int main(int argc, char *argv[]) {
  flag = parce_in(argc, argv, flag);
  if (flag.error == 1) {
    printf("s21_cat: illegal option\nusage: cat [-benstuv] [file ...]\n");
  } else {
    file_elaboration(my_optind, argc, argv, flag);
  }
  return 0;
}
