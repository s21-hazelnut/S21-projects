#ifndef SRC_GRAPH_S21_GRAPH_H_
#define SRC_GRAPH_S21_GRAPH_H_

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "s21_matrix.h"

namespace s21 {

class Graph {
 public:
  class Edge {
   public:
    Edge(int v1, int v2) : vertex1{v1}, vertex2{v2} {}

    size_t operator()(const Edge& e) const {
      return std::hash<int>()(e.vertex1) ^ std::hash<int>()(e.vertex2);
    }
    bool operator==(const Edge& other) const {
      return vertex1 == other.vertex1 && vertex2 == other.vertex2;
    };

   private:
    int vertex1;
    int vertex2;
  };

  Graph();
  explicit Graph(const std::string& filename);
  explicit Graph(int count_vertices);
  ~Graph();

  int operator()(const int& i, const int& j);
  void LoadGraphFromFile(const std::string& filename);
  void ExportGraphToDot(std::string filename);
  int GetCountOfVertexes();
  void AddVertex();
  void SetWeight(int vertex1, int vertex2, int weight);
  int Weight(int vertex1, int vertex2);
  void Print();
  S21Matrix GetMatrix();

 private:
  std::shared_ptr<S21Matrix> matrix_adjacency_;
};

}  // namespace s21

namespace std {
template <>
struct hash<s21::Graph::Edge> {
  std::size_t operator()(const s21::Graph::Edge& e) const noexcept {
    return e(e);
  }
};
}  // namespace std

#endif  // SRC_GRAPH_S21_GRAPH_H_
