#ifndef SRC_GRAPH_ALGORITHMS_S21_LEAST_SPANNING_TREE_H_
#define SRC_GRAPH_ALGORITHMS_S21_LEAST_SPANNING_TREE_H_

#include <ctime>
#include <random>
#include <set>
#include <vector>

#include "s21_graph.h"

namespace s21 {

class SpanningTree {
 public:
  explicit SpanningTree(Graph& graph);
  ~SpanningTree();
  S21Matrix GetLeastSpanningTree();

 private:
  struct Edge {
    int vertex_from;
    int vertex_to;
    int weight;
  };

  void Init();
  void FromVectorToMatrix();
  void UnvisitToVisit(int vertex);
  std::default_random_engine& RandomEngine();

  Graph graph_;
  S21Matrix spanning_tree_;
  std::set<int> visited_;
  std::set<int> unvisited_;
  std::vector<Edge> tree_edges_;
  int vertices_count_;
};

}  // namespace s21

#endif  // SRC_GRAPH_ALGORITHMS_S21_LEAST_SPANNING_TREE_H_