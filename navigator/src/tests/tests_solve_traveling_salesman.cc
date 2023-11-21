#include <numeric>
#include <vector>

#include "gtest/gtest.h"
#include "s21_graph.h"
#include "s21_solve_traveling_salesman.h"
#include "s21_tsm_result.h"

TEST(SolveTravelingSalesmanTest, FindDistance1) {
  s21::Graph graph;
  graph.LoadGraphFromFile("data/5.txt");
  s21::SolveTravelingSalesman solver(graph);
  s21::TsmResult result = solver.GetSolveTravelingSalesmanProblemResult();

  ASSERT_DOUBLE_EQ(result.distance, 48);
}

TEST(SolveTravelingSalesmanTest, FindDistance2) {
  s21::Graph graph;
  graph.LoadGraphFromFile("data/8.txt");
  s21::SolveTravelingSalesman solver(graph);
  s21::TsmResult result = solver.GetSolveTravelingSalesmanProblemResult();

  double expected_min_distance = 4900;
  double expected_max_distance = 5200;

  ASSERT_GE(result.distance, expected_min_distance);
  ASSERT_LE(result.distance, expected_max_distance);
}

TEST(SolveTravelingSalesmanTest, FindDistance3) {
  s21::Graph graph;
  graph.LoadGraphFromFile("data/3.txt");
  s21::SolveTravelingSalesman solver(graph);
  s21::TsmResult result = solver.GetSolveTravelingSalesmanProblemResult();

  ASSERT_DOUBLE_EQ(result.distance, std::numeric_limits<double>::infinity());
}

TEST(SolveTravelingSalesmanTest, FindDistance4) {
  s21::Graph graph;
  graph.LoadGraphFromFile("data/1.txt");
  s21::SolveTravelingSalesman solver(graph);
  s21::TsmResult result = solver.GetSolveTravelingSalesmanProblemResult();

  double expected_min_distance = 253;
  double expected_max_distance = 270;

  ASSERT_GE(result.distance, expected_min_distance);
  ASSERT_LE(result.distance, expected_max_distance);
}

TEST(SolveTravelingSalesmanTest, FindDistance5) {
  s21::Graph graph;
  graph.LoadGraphFromFile("data/6.txt");
  s21::SolveTravelingSalesman solver(graph);
  s21::TsmResult result = solver.GetSolveTravelingSalesmanProblemResult();

  ASSERT_DOUBLE_EQ(result.distance, 19);
}

TEST(SolveTravelingSalesmanTest, FindDistance6) {
  s21::Graph graph;
  graph.LoadGraphFromFile("data/9.txt");
  s21::SolveTravelingSalesman solver(graph);
  s21::TsmResult result = solver.GetSolveTravelingSalesmanProblemResult();

  ASSERT_DOUBLE_EQ(result.distance, 44);
}

TEST(SolveTravelingSalesmanTest, FindPath1) {
  s21::Graph graph;
  graph.LoadGraphFromFile("data/6.txt");
  std::vector<int> path1{1, 2, 3, 4, 1};
  std::vector<int> path2{2, 3, 4, 1, 2};
  std::vector<int> path3{3, 4, 1, 2, 3};
  std::vector<int> path4{4, 1, 2, 3, 4};
  s21::SolveTravelingSalesman solver(graph);
  s21::TsmResult result = solver.GetSolveTravelingSalesmanProblemResult();
  if (result.path[0] == 1) {
    ASSERT_EQ(result.path, path1);
  } else if (result.path[0] == 2) {
    ASSERT_EQ(result.path, path2);
  } else if (result.path[0] == 3) {
    ASSERT_EQ(result.path, path3);
  } else if (result.path[0] == 4) {
    ASSERT_EQ(result.path, path4);
  }
}
