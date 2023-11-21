#ifndef SRC_GRAPH_ALGORITHMS_S21_FLOYD_WARSHALL_H_
#define SRC_GRAPH_ALGORITHMS_S21_FLOYD_WARSHALL_H_

#include "s21_graph.h"
#include "s21_matrix.h"

namespace s21 {
class FloydWarshall {
 public:
  explicit FloydWarshall(Graph& graph);
  ~FloydWarshall();

  S21Matrix GetShortestPathsBetweenAllVertices();

 private:
  void InitMatrix(Graph& graph);
  void DeleteInfInMatrix();

  S21Matrix weights_;
};

}  // namespace s21

#endif  // SRC_GRAPH_ALGORITHMS_S21_FLOYD_WARSHALL_H_