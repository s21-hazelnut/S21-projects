#include "tests_breadth_search.cc"
#include "tests_depth_search.cc"
#include "tests_dijkstra.cc"
#include "tests_floyd_warshall.cc"
#include "tests_graph.cc"
#include "tests_least_spanning_tree.cc"
#include "tests_solve_traveling_salesman.cc"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
