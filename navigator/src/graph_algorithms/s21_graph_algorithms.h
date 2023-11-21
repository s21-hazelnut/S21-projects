#ifndef SRC_GRAPH_ALGORITHMS_S21_GRAPH_ALGORITHMS_H_
#define SRC_GRAPH_ALGORITHMS_S21_GRAPH_ALGORITHMS_H_

#include <string>
#include <vector>

#include "s21_breadth_search.h"
#include "s21_depth_search.h"
#include "s21_dijkstra.h"
#include "s21_floyd_warshall.h"
#include "s21_graph.h"
#include "s21_least_spanning_tree.h"
#include "s21_queue.h"
#include "s21_solve_traveling_salesman.h"
#include "s21_stack.h"
#include "s21_tsm_result.h"

namespace s21 {

class GraphAlgorithms {
 public:
  GraphAlgorithms();
  ~GraphAlgorithms();
  std::vector<int> DepthFirstSearch(Graph &graph, int start_vertex);
  std::vector<int> BreadthFirstSearch(Graph &graph, int start_vertex);
  int GetShortestPathBetweenVertices(Graph &graph, int vertex1, int vertex2);
  S21Matrix GetShortestPathsBetweenAllVertices(Graph &graph);
  S21Matrix LeastSpanningTree(Graph &graph);
  TsmResult SolveTravelingSalesmanProblem(Graph &graph);

 private:
};

}  // namespace s21

#endif  // SRC_GRAPH_ALGORITHMS_S21_GRAPH_ALGORITHMS_H_
