#ifndef SRC_GRAPH_ALGORITHMS_S21_BREADTH_SEARCH_H_
#define SRC_GRAPH_ALGORITHMS_S21_BREADTH_SEARCH_H_

#include <vector>

#include "s21_graph.h"
#include "s21_queue.h"
namespace s21 {

class BreadthSearch {
 public:
  explicit BreadthSearch(Graph& graph);
  ~BreadthSearch();
  std::vector<int> GetBreadthSearchResult(const int& start_vertex);

 private:
  void InitVisited();

  Graph graph_;
  std::vector<bool> visited_vert_;
};

}  // namespace s21

#endif  // SRC_GRAPH_ALGORITHMS_S21_BREADTH_SEARCH_H_