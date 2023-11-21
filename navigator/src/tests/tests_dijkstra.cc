#include "gtest/gtest.h"
#include "s21_dijkstra.h"
#include "s21_graph.h"

TEST(DijkstraTest, Test1) {
  s21::Graph graph("data/7.txt");
  s21::Dijkstra alg(graph);
  ASSERT_ANY_THROW(alg.GetShortestPath(0, 1));
}

TEST(DijkstraTest, Test2) {
  s21::Graph graph("data/7.txt");
  s21::Dijkstra alg(graph);
  ASSERT_ANY_THROW(alg.GetShortestPath(1, 7));
}

TEST(DijkstraTest, Test3) {
  s21::Graph graph("data/7.txt");
  s21::Dijkstra alg(graph);
  ASSERT_ANY_THROW(alg.GetShortestPath(-5, 3));
}

TEST(DijkstraTest, Test4) {
  s21::Graph graph("data/7.txt");
  s21::Dijkstra alg(graph);
  int res = alg.GetShortestPath(1, 1);
  int test_res = 0;
  EXPECT_EQ(res, test_res);
}

TEST(DijkstraTest, Test5) {
  s21::Graph graph("data/7.txt");
  s21::Dijkstra alg(graph);
  int res = alg.GetShortestPath(1, 5);
  int test_res = 16;
  EXPECT_EQ(res, test_res);
}

TEST(DijkstraTest, Test6) {
  s21::Graph graph("data/7.txt");
  s21::Dijkstra alg(graph);
  int res = alg.GetShortestPath(4, 6);
  int test_res = 13;
  EXPECT_EQ(res, test_res);
}

TEST(DijkstraTest, Test7) {
  s21::Graph graph("data/7.txt");
  s21::Dijkstra alg(graph);
  int res = alg.GetShortestPath(5, 2);
  int test_res = 15;
  EXPECT_EQ(res, test_res);
}
