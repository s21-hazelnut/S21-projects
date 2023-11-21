DIRGRAPH=graph
DIRALGO=graph_algorithms
DIRBUILD=build
DIRINTERFACE=console_interface
DIRSUPPORT=support
DIRTEST=tests
DIRCOVERAGE=coverage
GCC=g++
ERROR=-Wall -Werror -Wextra
CFLAGS=-g --std=c++17 $(ERROR) --coverage
MFLAGS=-c -MMD -MP
LIBGRAPH=$(DIRBUILD)/s21_graph.a
LIBALGO=$(DIRBUILD)/s21_graph_algorithms.a
OUT=$(DIRBUILD)/main.out
NAME_TESTS=tests
TEST=$(DIRBUILD)/$(NAME_TESTS).out

S_SUPPORT:=$(wildcard $(DIRSUPPORT)/*.cc)
S_GRAPH:=$(wildcard $(DIRGRAPH)/*.cc)
S_ALGO:=$(wildcard $(DIRALGO)/*.cc)
S_INTERFACE:=$(wildcard $(DIRINTERFACE)/*.cc)
S_MAIN=main.cc
S_TEST=$(DIRTEST)/$(NAME_TESTS).cc

O_SUPPORT=$(addprefix $(DIRBUILD)/,$(notdir $(patsubst %.cc,%.o,$(S_SUPPORT))))
O_GRAPH=$(addprefix $(DIRBUILD)/,$(notdir $(patsubst %.cc,%.o,$(S_GRAPH))))
O_ALGO=$(addprefix $(DIRBUILD)/,$(notdir $(patsubst %.cc,%.o,$(S_ALGO))))
O_INTERFACE=$(addprefix $(DIRBUILD)/,$(notdir $(patsubst %.cc,%.o,$(S_INTERFACE))))
O_MAIN=$(addprefix $(DIRBUILD)/,$(notdir $(patsubst %.cc,%.o,$(S_MAIN))))
O_TEST=$(addprefix $(DIRBUILD)/,$(notdir $(patsubst %.cc,%.o,$(S_TEST))))

.PHONY: all makedir exec clean cpplint

all: makedir build exec

makedir:
	@mkdir -p build

build: $(O_INTERFACE) $(O_MAIN) $(LIBGRAPH) $(LIBALGO)
	$(GCC) $(CFLAGS) -o $(OUT) $^

$(O_SUPPORT): $(DIRBUILD)/%.o: $(DIRSUPPORT)/%.cc
	$(GCC) $(CFLAGS) $(MFLAGS) -I$(DIRSUPPORT) $< -o $@

$(O_GRAPH): $(DIRBUILD)/%.o: $(DIRGRAPH)/%.cc
	$(GCC) $(CFLAGS) $(MFLAGS) -I$(DIRGRAPH) -I$(DIRSUPPORT) $< -o $@

$(O_ALGO): $(DIRBUILD)/%.o: $(DIRALGO)/%.cc
	$(GCC) $(CFLAGS) $(MFLAGS) -I$(DIRALGO) -I$(DIRGRAPH) -I$(DIRSUPPORT) $< -o $@

$(LIBGRAPH): $(O_GRAPH) $(O_SUPPORT)
	ar rcs $@ $^

$(LIBALGO): $(O_GRAPH) $(O_ALGO)
	ar rcs $@ $^

$(O_INTERFACE): $(DIRBUILD)/%.o: $(DIRINTERFACE)/%.cc
	$(GCC) $(CFLAGS) $(MFLAGS) -I$(DIRALGO) -I$(DIRGRAPH) -I$(DIRSUPPORT) $< -o $@

$(O_MAIN): $(DIRBUILD)/%.o: %.cc 
	$(GCC) $(CFLAGS) $(MFLAGS) -I$(DIRALGO) -I$(DIRGRAPH) -I$(DIRSUPPORT) -I$(DIRINTERFACE) $< -o $@

$(O_TEST): $(DIRBUILD)/%.o: $(DIRTEST)/%.cc 
	$(GCC) $(CFLAGS) $(MFLAGS) -I$(DIRALGO) -I$(DIRGRAPH) -I$(DIRSUPPORT) $< -o $@

exec:
	./$(OUT)

gcov_report: tests
	@mkdir -p $(DIRCOVERAGE)
	lcov --directory . --config-file lcov_exclude.conf --capture --no-external \
	--output-file $(DIRCOVERAGE)/coverage.info
	genhtml $(DIRCOVERAGE)/coverage.info --output-directory $(DIRCOVERAGE)/coverage_report
ifeq ($(OS), Darwin)	
	open -a "Google Chrome" $(DIRCOVERAGE)/coverage_report/index.html
else
	google-chrome $(DIRCOVERAGE)/coverage_report/index.html
endif	

tests: makedir test

test: $(O_TEST) $(LIBGRAPH) $(LIBALGO)
	$(GCC) $(CFLAGS) $^ -lgtest -o $(TEST)
	$(TEST)

clean:
	rm -rf build $(DIRCOVERAGE)

cpplint:
	@cp materials/linters/.clang-format .
	clang-format -n */*.cc */*.h
	@rm -rf .clang-format

-include $(DIRBUILD)/*.d
