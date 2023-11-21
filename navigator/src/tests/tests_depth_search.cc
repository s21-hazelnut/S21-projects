#include <vector>

#include "gtest/gtest.h"
#include "s21_depth_search.h"
#include "s21_graph.h"

TEST(DepthTest, Test1) {
  s21::Graph graph("data/4.txt");
  s21::DepthSearch DepthForTest(graph);
  std::vector<int> res = DepthForTest.GetDepthSearchResult(5);
  std::vector<int> test_res{5, 2, 1, 3, 4, 6, 7, 9, 8, 10};
  EXPECT_EQ(res, test_res);
}

TEST(DepthTest, Test2) {
  s21::Graph graph("data/3.txt");
  s21::DepthSearch DepthForTest(graph);
  std::vector<int> res = DepthForTest.GetDepthSearchResult(2);
  std::vector<int> test_res{2, 1, 3, 6, 9, 5, 10, 7, 4, 8};
  EXPECT_EQ(res, test_res);
}

TEST(DepthTest, Test3) {
  s21::Graph graph("data/3.txt");
  s21::DepthSearch DepthForTest(graph);
  std::vector<int> res = DepthForTest.GetDepthSearchResult(0);
  std::vector<int> test_res;
  EXPECT_EQ(res, test_res);
}

TEST(DepthTest, Test4) {
  s21::Graph graph("data/3.txt");
  s21::DepthSearch DepthForTest(graph);
  std::vector<int> res = DepthForTest.GetDepthSearchResult(11);
  std::vector<int> test_res;
  EXPECT_EQ(res, test_res);
}
