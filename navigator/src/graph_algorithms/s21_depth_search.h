#ifndef SRC_GRAPH_ALGORITHMS_S21_DEPTH_SEARCH_H_
#define SRC_GRAPH_ALGORITHMS_S21_DEPTH_SEARCH_H_

#include <queue>
#include <unordered_set>
#include <vector>

#include "s21_graph.h"
#include "s21_stack.h"

namespace s21 {

class DepthSearch {
 public:
  explicit DepthSearch(Graph& graph);
  ~DepthSearch();
  std::vector<int> GetDepthSearchResult(const int& start_vertex);

 private:
  void InitVisited();

  Graph graph_;

  std::vector<bool> visited_vert_;
};

}  // namespace s21

#endif  // SRC_GRAPH_ALGORITHMS_S21_DEPTH_SEARCH_H_