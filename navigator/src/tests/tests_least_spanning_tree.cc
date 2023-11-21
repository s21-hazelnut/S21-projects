#include <gtest/gtest.h>

#include "s21_graph.h"
#include "s21_least_spanning_tree.h"
#include "s21_matrix.h"

TEST(SpanningTreeTest, CreateSpanningTree) {
  s21::Graph graph;
  graph.LoadGraphFromFile("data/6.txt");
  s21::SpanningTree spanning_tree(graph);
  S21Matrix mst = spanning_tree.GetLeastSpanningTree();

  ASSERT_EQ(mst.GetElement(0, 0), 0);
  ASSERT_EQ(mst.GetElement(1, 1), 0);
  ASSERT_EQ(mst.GetElement(2, 2), 0);
  ASSERT_EQ(mst.GetElement(3, 3), 0);
}

TEST(SpanningTreeTest, CreateSpanningTree1) {
  s21::Graph graph;
  graph.LoadGraphFromFile("data/5.txt");
  s21::SpanningTree spanning_tree(graph);
  S21Matrix mst = spanning_tree.GetLeastSpanningTree();

  ASSERT_EQ(mst.GetElement(0, 0), 0);
  ASSERT_EQ(mst.GetElement(1, 1), 0);
  ASSERT_EQ(mst.GetElement(2, 2), 0);
  ASSERT_EQ(mst.GetElement(3, 3), 0);
  ASSERT_EQ(mst.GetElement(4, 4), 0);
  ASSERT_EQ(mst.GetElement(5, 5), 0);

  ASSERT_EQ(mst.GetElement(0, 1), 7);
  ASSERT_EQ(mst.GetElement(0, 2), 9);
  ASSERT_EQ(mst.GetElement(2, 5), 2);
  ASSERT_EQ(mst.GetElement(3, 4), 6);
  ASSERT_EQ(mst.GetElement(4, 5), 9);
}
