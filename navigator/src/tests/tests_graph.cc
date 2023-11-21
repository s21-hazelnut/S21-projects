#include <gtest/gtest.h>

#include <fstream>
#include <string>

#include "s21_graph.h"
#include "s21_matrix.h"

TEST(GraphTest, Constructor) {
  s21::Graph graph;
  ASSERT_TRUE(true);
}
TEST(GraphTest, ConstructGraphWithVertices) {
  s21::Graph graph(5);
  ASSERT_EQ(graph.GetCountOfVertexes(), 5);
  for (int i = 1; i <= 5; ++i) {
    for (int j = 1; j <= 5; ++j) {
      ASSERT_EQ(graph(i, j), 0);
    }
  }
}
TEST(GraphTest, PrintGraph) {
  s21::Graph graph;
  graph.LoadGraphFromFile("data/6.txt");
  ASSERT_NO_THROW(graph.Print());
}
TEST(GraphTest, GetMatrix) {
  s21::Graph graph;
  graph.LoadGraphFromFile("data/6.txt");

  S21Matrix matrix = graph.GetMatrix();

  ASSERT_EQ(matrix.GetRows(), graph.GetCountOfVertexes());
  ASSERT_EQ(matrix.GetCols(), graph.GetCountOfVertexes());

  for (int i = 1; i <= matrix.GetRows(); ++i) {
    for (int j = 1; j <= matrix.GetCols(); ++j) {
      ASSERT_EQ(matrix.GetElement(i - 1, j - 1), graph(i, j));
    }
  }
}

TEST(GraphTest, LoadGraphFromFile) {
  s21::Graph graph;
  ASSERT_NO_THROW(graph.LoadGraphFromFile("data/6.txt"));
}
TEST(GraphTest, LoadNonExistentFile) {
  s21::Graph graph;
  ASSERT_THROW(graph.LoadGraphFromFile("non_existent_file.txt"),
               std::ios_base::failure);
}
TEST(GraphTest, LoadIncorrectFile) {
  s21::Graph graph;
  std::ofstream temp_file("incorrect_file.txt");
  temp_file << "This is an incorrect file content." << std::endl;
  temp_file.close();

  ASSERT_THROW(graph.LoadGraphFromFile("incorrect_file.txt"),
               std::invalid_argument);

  std::remove("incorrect_file.txt");
}
TEST(GraphTest, ExportGraphToDotWith6Vertices) {
  s21::Graph graph;
  graph.LoadGraphFromFile("data/6.txt");

  std::string filename{"tests/exported_graph_6.dot"};
  ASSERT_NO_THROW(graph.ExportGraphToDot(filename));

  std::ifstream file(filename);
  ASSERT_TRUE(file.good());

  std::string text_file;
  std::string tmp_text;
  while (getline(file, tmp_text)) {
    text_file.append(tmp_text);
  }
  std::string text_test{
      "digraph {  1 -> 2 [label = 7; len = 7;];  1 -> 3 [label = 9; len = 9;]; "
      " 2 -> 1 [label = 8; len = 8;];  2 -> 3 [label = 4; len = 4;];  2 -> 4 "
      "[label = 2; len = 2;];  3 -> 1 [label = 6; len = 6;];  3 -> 4 [label = "
      "3; len = 3;];  4 -> 1 [label = 5; len = 5;];  4 -> 2 [label = 5; len = "
      "5;];}"};
  ASSERT_EQ(text_file, text_test);

  std::remove(filename.c_str());
}

TEST(GraphTest, BracketOperator) {
  s21::Graph graph;
  graph.LoadGraphFromFile("data/6.txt");
  ASSERT_EQ(graph(1, 2), 7);
  ASSERT_EQ(graph(2, 3), 4);
  ASSERT_EQ(graph(4, 2), 5);
  ASSERT_EQ(graph(3, 4), 3);
}

TEST(GraphTest, LoadGraphFromFileWith6Vertices) {
  s21::Graph graph;
  ASSERT_NO_THROW(graph.LoadGraphFromFile("data/6.txt"));
  ASSERT_EQ(graph.GetCountOfVertexes(), 4);
  ASSERT_EQ(graph.Weight(1, 2), 7);
  ASSERT_EQ(graph.Weight(1, 3), 9);
  ASSERT_EQ(graph.Weight(2, 1), 8);
  ASSERT_EQ(graph.Weight(2, 3), 4);
  ASSERT_EQ(graph.Weight(2, 4), 2);
  ASSERT_EQ(graph.Weight(3, 1), 6);
  ASSERT_EQ(graph.Weight(3, 4), 3);
  ASSERT_EQ(graph.Weight(4, 1), 5);
  ASSERT_EQ(graph.Weight(4, 2), 5);
}

TEST(GraphTest, SetWeightWith6Vertices) {
  s21::Graph graph;
  graph.LoadGraphFromFile("data/6.txt");
  graph.SetWeight(1, 2, 10);
  ASSERT_EQ(graph.Weight(1, 2), 10);
  ASSERT_EQ(graph.Weight(2, 1), 10);
  ASSERT_EQ(graph.Weight(1, 3), 9);
  ASSERT_EQ(graph.Weight(2, 3), 4);
}
TEST(GraphTest, AddVertexWith6Vertices) {
  s21::Graph graph;
  graph.LoadGraphFromFile("data/6.txt");
  graph.AddVertex();
  ASSERT_EQ(graph.GetCountOfVertexes(), 5);
  for (int i = 1; i <= 4; ++i) {
    ASSERT_EQ(graph.Weight(5, i), 0);
    ASSERT_EQ(graph.Weight(i, 5), 0);
  }
}
