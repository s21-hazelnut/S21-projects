#include <vector>

#include "gtest/gtest.h"
#include "s21_breadth_search.h"
#include "s21_graph.h"

TEST(BreadthTest, Test1) {
  s21::Graph graph("data/4.txt");
  s21::BreadthSearch BreadthForTest(graph);
  std::vector<int> res = BreadthForTest.GetBreadthSearchResult(5);
  std::vector<int> test_res{5, 2, 3, 6, 7, 8, 1, 4, 9, 10};
  EXPECT_EQ(res, test_res);
}

TEST(BreadthTest, Test2) {
  s21::Graph graph("data/3.txt");
  s21::BreadthSearch BreadthForTest(graph);
  std::vector<int> res = BreadthForTest.GetBreadthSearchResult(2);
  std::vector<int> test_res{2, 1, 4, 6, 10, 3, 5, 7, 8, 9};
  EXPECT_EQ(res, test_res);
}

TEST(BreadthTest, Test3) {
  s21::Graph graph("data/3.txt");
  s21::BreadthSearch BreadthForTest(graph);
  std::vector<int> res = BreadthForTest.GetBreadthSearchResult(0);
  std::vector<int> test_res;
  EXPECT_EQ(res, test_res);
}

TEST(BreadthTest, Test4) {
  s21::Graph graph("data/3.txt");
  s21::BreadthSearch BreadthForTest(graph);
  std::vector<int> res = BreadthForTest.GetBreadthSearchResult(11);
  std::vector<int> test_res;
  EXPECT_EQ(res, test_res);
}
