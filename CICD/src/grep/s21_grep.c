#include "s21_grep.h"

struct Flags parce_in(int argc, char *argv[], struct Flags flag) {
  char *short_flags = "eivclnhsf:o";
  struct option long_flags[] = {{NULL, 0, NULL, 0}};
  int rezult;
  int opterr = 0;
  while ((rezult = getopt_long(argc, argv, short_flags, long_flags, &opterr)) !=
         -1) {
    switch (rezult) {
      case 'e': {
        flag.e = 1;
        break;
      };
      case 'i': {
        flag.i = 1;
        break;
      };
      case 'v': {
        flag.v = 1;
        break;
      };
      case 'c': {
        flag.c = 1;
        break;
      };
      case 'l': {
        flag.l = 1;
        break;
      };
      case 'n': {
        flag.n = 1;
        break;
      };
      case 'h': {
        flag.h = 1;
        break;
      };
      case 's': {
        flag.s = 1;
        break;
      };
      case 'f': {
        flag.f = 1;
        flag.file_for_f = optarg;
        break;
      };
      case 'o': {
        flag.o = 1;
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

char *get_line(FILE *file, char rezult[]) {
  int r = 0;
  for (int i = 0; i < 10000; i++) rezult[i] = '\n';
  char symbol = '0';
  while (symbol != '\n' && !feof(file)) {
    symbol = fgetc(file);
    rezult[r] = symbol;
    r++;
  }
  rezult[r] = '\0';
  return &rezult[0];
}

char *get_f_pattern_from_file(struct Flags flag, char *pattern_f) {
  FILE *patternFile = fopen(flag.file_for_f, "r");
  if (patternFile) {
    int j = 0;
    int start_pattern = 1;
    while (!feof(patternFile)) {
      char sym = fgetc(patternFile);
      while ((sym == '\n') && start_pattern) {
        pattern_f[j] = '.';
        j++;
        sym = fgetc(patternFile);
      }
      if (sym == '\n') {
        pattern_f[j] = '|';
        j++;
      } else {
        pattern_f[j] = sym;
        j++;
      }
      start_pattern = 0;
    }
    j--;
    pattern_f[j] = '\0';
    if (pattern_f[j - 1] == '|') pattern_f[j - 1] = '\0';
    fclose(patternFile);
  }
  return (&pattern_f[0]);
}

void file_elaboration(int my_optind, int argc, char *argv[],
                      struct Flags flag) {
  char *search_line;
  if (!flag.f) {
    search_line = argv[my_optind];
    my_optind++;
  }
  int many_files = (argc - my_optind > 1) ? 1 : 0;
  for (int i = my_optind; i < argc; i++) {
    FILE *file;
    char *dest_file_name = argv[i];
    file = fopen(dest_file_name, "r");
    if (file) {
      int NOprint = 0;
      unsigned long line_num = 0;
      int last_line_is_empty = 0;
      int line_sum = 0;
      int anti_line_sum = 0;
      while (!feof(file)) {
        char rezult[10000];
        char *line = get_line(file, rezult);
        regex_t regex_struct;
        regmatch_t matched[0];
        int par;
        line_num++;
        if (flag.f) {
          char *pattern = get_f_pattern_from_file(flag, pattern_f);
          par = regcomp(&regex_struct, pattern, REG_EXTENDED);
        } else if (flag.i) {
          par = regcomp(&regex_struct, search_line, REG_ICASE);
        } else {
          par = regcomp(&regex_struct, search_line, REG_EXTENDED);
        }
        if (!par) {
          int comp;
          size_t matches = 0;
          comp = regexec(&regex_struct, line, matches, matched, 0);
          if (!comp) {
            line_sum++;
          } else {
            anti_line_sum++;
          }
          if (strlen(line) == 1)
            last_line_is_empty = 1;
          else
            last_line_is_empty = 0;
          print_line(flag, many_files, NOprint, last_line_is_empty, line_num,
                     file, dest_file_name, line, comp);
        } else {
          printf("Could not compile regex\n");
          continue;
        }
        regfree(&regex_struct);
      }
      if (last_line_is_empty) anti_line_sum--;
      print_if_c_or_l(flag, many_files, anti_line_sum, line_sum,
                      dest_file_name);
      fclose(file);
    } else {
      if (!flag.s) printf("grep: %s: No such file or directory\n", argv[i]);
    }
  }
}

void print_line(struct Flags flag, int many_files, int NOprint,
                int last_line_is_empty, int line_num, FILE *file,
                char *dest_file_name, char *line, int comp) {
  if (flag.l) NOprint = 1;
  if (flag.c) {
    NOprint = 1;
  }
  if ((comp && flag.v) || (!comp && !flag.v)) {
    if (many_files && !flag.h && !NOprint &&
        !(feof(file) && last_line_is_empty))
      printf("%s:", dest_file_name);
    if (!NOprint && flag.n && !(feof(file) && last_line_is_empty))
      printf("%d:", line_num);
    if (!NOprint && !(feof(file) && ((flag.v || flag.f) ? last_line_is_empty
                                                        : !last_line_is_empty)))
      printf("%.*s\n", (int)(strlen(line) - 1), line);
  }
}
void print_if_c_or_l(struct Flags flag, int many_files, int anti_line_sum,
                     int line_sum, char *dest_file_name) {
  if (flag.c) {
    if (many_files && !flag.h) printf("%s:", dest_file_name);
    if (flag.v) {
      if (!flag.l)
        printf("%d\n", anti_line_sum);
      else
        printf("%d\n", (anti_line_sum ? 1 : 0));
    } else {
      if (!flag.l)
        printf("%d\n", line_sum);
      else
        printf("%d\n", (line_sum ? 1 : 0));
    }
  }
  if (flag.l && ((line_sum && !flag.v) || (anti_line_sum && flag.v))) {
    printf("%s\n", dest_file_name);
  }
}

int main(int argc, char *argv[]) {
  flag = parce_in(argc, argv, flag);
  if (flag.error == 1) {
    printf("grep: illegal option\nusage: cat [-benstuv] [file ...]\n");
  } else {
    file_elaboration(my_optind, argc, argv, flag);
  }
  return 0;
}
