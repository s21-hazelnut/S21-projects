CC=gcc
CFLAGS=-Wall -Wextra -Werror
GCOV=-fprofile-arcs -ftest-coverage
OS=$(shell uname)
ifeq ($(OS), Linux)
	LIBS=-lcheck -lpthread -lrt -lm -lsubunit
	LEAKS=CK_FORK=no valgrind -s --tool=memcheck --leak-check=full --show-leak-kinds=all --show-reachable=yes ./s21_cat -b -e -n -s -t -v bytes.txt
	LEAKS=CK_FORK=no valgrind -s --tool=memcheck --leak-check=full --show-leak-kinds=all --show-reachable=yes ./s21_cat -b -e -n -s -t test.txt
else
	LIBS=-lcheck
	LEAKS=CK_FORK=no leaks --atExit -- ./s21_cat -b -e -n -s -t -v bytes.txt
	LEAKS=CK_FORK=no leaks --atExit -- ./s21_cat -b -e -n -s -t test.txt
endif

all: clean s21_cat
test: s21_cat
	sh cat_check.sh

cat_test: s21_cat
	cat Makefile > out1.txt && ./s21_cat Makefile > out2.txt && diff out1.txt out2.txt
	cat -b Makefile > out1.txt && ./s21_cat -b Makefile > out2.txt && diff out1.txt out2.txt
	cat -e Makefile > out1.txt && ./s21_cat -e Makefile > out2.txt && diff out1.txt out2.txt
	cat -n Makefile > out1.txt && ./s21_cat -n Makefile > out2.txt && diff out1.txt out2.txt
	cat -s Makefile > out1.txt && ./s21_cat -s Makefile > out2.txt && diff out1.txt out2.txt
	cat -t Makefile > out1.txt && ./s21_cat -t Makefile > out2.txt && diff out1.txt out2.txt
	cat -v Makefile > out1.txt && ./s21_cat -v Makefile > out2.txt && diff out1.txt out2.txt
	cat -n -v Makefile > out1.txt && ./s21_cat -n -v Makefile > out2.txt && diff out1.txt out2.txt
	cat -s -b Makefile > out1.txt && ./s21_cat -s -b Makefile > out2.txt && diff out1.txt out2.txt
	cat -t -e Makefile > out1.txt && ./s21_cat -t -e Makefile > out2.txt && diff out1.txt out2.txt
	cat -v -b -e -n -s -t Makefile > out1.txt && ./s21_cat -v -b -e -n -s -t Makefile > out2.txt && diff out1.txt out2.txt
	rm -rf out1.txt out2.txt

gcov_report: s21_cat
	$(CC) $(CFLAGS) s21_cat.c -L. --coverage -lcheck -lpthread -lm -lgcov -o test_cov
	rm -rf *.o
	./test_cov -s -t -v -b test.txt
	./test_cov -b -e -n -s -t -v  bytes.txt
	./test_cov -b bytes.txt
	./test_cov -b bytegdffs.txt
	./test_cov -w bytes.txt
	./test_cov -s -t -v test.txt
	./test_cov -n test.txt
	./test_cov -b -e -n -s -t -v -E test.txt
	./test_cov -b -e -n -T test.txt
	gcov *.gcda
	lcov -t "gcov_report" -o gcov_report.info -c -d .
	mkdir -p ./report
	genhtml -o report gcov_report.info
	open ./report/index.html

s21_cat:
	$(CC) $(CFLAGS) s21_cat.c -o s21_cat

clean:
	rm -rf *.0 *.a *.gcno *.gcda *.info *.out report *.dSYM *.gch *.o .vscode .DS_Store *.c.gcov test_cov s21_cat CPPLINT.cfg ../.DS_Store ../.vscode log.txt

codestyle:
	cp ../../materials/linters/.clang-format ./
	clang-format -i *.c *.h
	clang-format -n *.h *.c
	rm -rf .clang-format

leaks: clean s21_cat
	$(LEAKS)

CPPcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem s21_cat.c *.c *.h

CH: clean codestyle CPPcheck leaks
	rm -rf *.0 *.a *.gcno *.gcda *.info *.out report *.dSYM *.gch *.o .vscode .DS_Store *.c.gcov test_cov s21_cat ../.DS_Store