#include "gtest/gtest.h"
#include "s21_floyd_warshall.h"
#include "s21_graph.h"
#include "s21_matrix.h"

TEST(FloydWarshallTest, Test1) {
  s21::Graph graph("data/7.txt");
  s21::FloydWarshall alg(graph);
  S21Matrix matrix = alg.GetShortestPathsBetweenAllVertices();
  S21Matrix test_matrix(6, 6, {0,  7,  9,  17, 16, 8, 7,  0,  10, 12, 17, 12,
                               6,  3,  0,  8,  7,  2, 17, 14, 11, 0,  6,  13,
                               18, 15, 12, 1,  0,  9, 9,  6,  3,  10, 9,  0});
  EXPECT_EQ(matrix, test_matrix);
}
