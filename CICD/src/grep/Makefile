CC=gcc
CFLAGS=-Wall -Wextra -Werror
GCOV=-fprofile-arcs -ftest-coverage
OS=$(shell uname)
ifeq ($(OS), Linux)
	LIBS=-lcheck -lpthread -lrt -lm -lsubunit
	LEAKS=CK_FORK=no valgrind -s --tool=memcheck --leak-check=full --show-leak-kinds=all --show-reachable=yes ./s21_grep for s21_grep.c
	LEAKS=CK_FORK=no valgrind -s --tool=memcheck --leak-check=full --show-leak-kinds=all --show-reachable=yes ./s21_grep -v -c -l -n -h -i for s21_grep.c
else
	LIBS=-lcheck
	LEAKS=CK_FORK=no leaks --atExit -- ./s21_grep for s21_grep.c
	LEAKS=CK_FORK=no leaks --atExit -- ./s21_grep -v -c -l -n -h -i for s21_grep.c
endif

all: clean s21_grep
test: s21_grep
	sh grep_check.sh

s21_grep:
	$(CC) $(CFLAGS) s21_grep.c -o s21_grep

grep_test: s21_grep
	grep -e make Makefile > out1.txt && ./s21_grep make Makefile > out2.txt && diff out1.txt out2.txt
	grep -i make Makefile Makefile> out1.txt && ./s21_grep -i make Makefile Makefile> out2.txt && diff out1.txt out2.txt
	grep -v make Makefile > out1.txt && ./s21_grep -v make Makefile > out2.txt && diff out1.txt out2.txt 
	grep -c make Makefile > out1.txt && ./s21_grep -c make Makefile > out2.txt && diff out1.txt out2.txt
	grep -l make Makefile > out1.txt && ./s21_grep -l make Makefile > out2.txt && diff out1.txt out2.txt
	grep -n make Makefile > out1.txt && ./s21_grep -n make Makefile > out2.txt && diff out1.txt out2.txt
	grep -i make Makefile Makefile> out1.txt && ./s21_grep -i make Makefile Makefile> out2.txt && diff out1.txt out2.txt
	grep -h make Makefile > out1.txt && ./s21_grep -h make Makefile > out2.txt && diff out1.txt out2.txt 
	grep -s make Makefile > out1.txt && ./s21_grep -s make Makefile > out2.txt && diff out1.txt out2.txt
	grep -l -n -e make Makefile > out1.txt && ./s21_grep -l -n -e make Makefile > out2.txt && diff out1.txt out2.txt
	grep -n -v make Makefile > out1.txt && ./s21_grep -n -v make Makefile > out2.txt && diff out1.txt out2.txt
	grep -f pattern.txt Makefile > out1.txt && ./s21_grep -f pattern.txt Makefile > out2.txt && diff out1.txt out2.txt
	rm -rf out1.txt out2.txt

gcov_report: s21_grep
	$(CC) s21_grep.c -L. --coverage -lcheck -lpthread -lm -lgcov -o test_cov
	rm -rf *.o
	./test_cov make Makefile
	./test_cov -i make Makefile Makefile
	./test_cov -v make Makefile
	./test_cov -c make Makefile
	./test_cov -l make Makefile
	./test_cov -n make Makefile
	./test_cov -i make Makefile Makefile
	./test_cov -h make Makefile 
	./test_cov -s make Makefile
	./test_cov -l -n -e make Makefile
	./test_cov -n -v make Makefile
	./test_cov -f pattern.txt Makefile
	gcov *.gcda
	lcov -t "gcov_report" -o gcov_report.info -c -d .
	mkdir -p ./report
	genhtml -o report gcov_report.info
	open ./report/index.html

clean:
	rm -rf *.0 *.a *.gcno *.gcda *.info *.out report *.dSYM *.gch *.o .vscode .DS_Store *.c.gcov test_cov s21_grep CPPLINT.cfg ../.vscode log.txt gre*.txt s21_gre*.txt

codestyle: clean
	cp ../../materials/linters/.clang-format ./
	clang-format -i *.c *.h
	clang-format -n *.h *.c
	rm -rf .clang-format

CPPcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem s21_grep.c *.c *.h

leaks: clean s21_grep
	$(LEAKS)

CH: clean codestyle CPPcheck leaks
	rm -rf *.0 *.a *.gcno *.gcda *.info *.out report *.dSYM *.gch *.o .vscode .DS_Store *.c.gcov test_cov s21_grep