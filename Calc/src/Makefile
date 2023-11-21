CC=g++
CFLAGS=-Wall -Wextra -Werror
GCOV=-fprofile-arcs -ftest-coverage
OS=$(shell uname)
ifeq ($(OS), Linux)
	LIBS=-lcheck -lpthread -lrt -lm -lsubunit
	LEAKS=CK_FORK=no valgrind -s --tool=memcheck --leak-check=full --show-leak-kinds=all --show-reachable=yes ./tests.out
	testS=$(CC) -g test.cpp s21_SmartCalc.a $(LIBS) -o tests.out
	# -lsubunit
else
	LIBS=-lcheck -lgtest -pthread
	LEAKS=CK_FORK=no leaks --atExit -- ./tests.out
	testS=$(CC) test.cpp s21_SmartCalc.a $(LIBS) -o tests.out
endif

all: clean s21_SmartCalc.a tests

install: dist
	cp -rf calc.pro *.h *.cpp *.ui ../build
	cd ../build; qmake calc.pro
	make -C ../build
	cp -rf ../build/calc.app  $(HOME)/Desktop

uninstall:
	@rm -rf $(HOME)/Desktop/calc.app
	@rm -rf ../build

dvi: 
	open dvi.html

dist:
	mkdir -p ../build
	tar -cvzf ../build/calc.tgz Makefile *.h *.pro *.user *.cpp *.ui *.html

tests: clean s21_SmartCalc.a
	$(testS)
	./tests.out

gcov_report: s21_SmartCalc.a
	$(CC) --coverage -c s21_SmartCalc.cpp
	ar rc s21_SmartCalc.a *.o
	$(CC) test.cpp -L. s21_SmartCalc.a -lcheck -lgtest -pthread -lm -lgcov -o test_cov
	rm -rf *.o
	./test_cov
	gcov *.gcda
	lcov -t "gcov_report" -o gcov_report.info -c -d .
	mkdir -p ./report
	genhtml -o report gcov_report.info
	open ./report/index.html

s21_SmartCalc.a:
	g++ -c s21_SmartCalc.cpp -o s21_SmartCalc.o
	ar rcs s21_SmartCalc.a s21_SmartCalc.o

clean:
	rm -rf *.0 *.a *.gcno *.gcda *.info *.out report *.dSYM *.gch *.o test2.out ./.vscode .vscode 
	rm -rf .DS_Store *.gcov test_cov CPPLINT.cfg s21_SmartCalc ../.DS_Store ../.vscode a.out

leaks: clean tests
	$(LEAKS)

style:
	cp ../materials/linters/.clang-format ./
# brew install clang-format
	clang-format -i *.cpp *.h
	clang-format -n *.h *.cpp
	rm -rf .clang-format

CPPcheck:
	cppcheck --std=c++17 --language=c++ --suppress=missingIncludeSystem s21_SmartCalc.cpp s21_SmartCalc.h

CH: clean leaks style CPPcheck
	rm -rf *.0 *.a *.gcno *.gcda *.info *.out report *.dSYM *.gch *.o test2.out ./.vscode .vscode 
	rm -rf .DS_Store *.gcov test_cov CPPLINT.cfg s21_SmartCalc ../.DS_Store ../.vscode a.out