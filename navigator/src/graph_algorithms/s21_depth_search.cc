#include "s21_depth_search.h"

s21::DepthSearch::DepthSearch(Graph& graph) : graph_{graph} {}

s21::DepthSearch::~DepthSearch() {}

void s21::DepthSearch::InitVisited() {
  visited_vert_.clear();
  for (int i = 0; i <= graph_.GetCountOfVertexes(); i++) {
    visited_vert_.push_back(false);
  }
}

std::vector<int> s21::DepthSearch::GetDepthSearchResult(
    const int& start_vertex) {
  std::vector<int> res;
  if (start_vertex < 1 || start_vertex > graph_.GetCountOfVertexes())
    return res;
  InitVisited();
  visited_vert_[start_vertex] = true;
  s21::stack<int> DFS_stack;
  DFS_stack.push(start_vertex);
  res.push_back(start_vertex);
  size_t step = 1;
  while (!DFS_stack.empty()) {
    int from = DFS_stack.pop();
    bool connect = false;
    for (int i = 1; i <= graph_.GetCountOfVertexes() && !connect; i++) {
      if (!visited_vert_[i] && graph_.Weight(from, i)) {
        visited_vert_[i] = true;
        DFS_stack.push(i);
        res.push_back(i);
        from = i;
        connect = true;
      }
    }
    int unvisited = 0;
    for (int i = 1; i <= graph_.GetCountOfVertexes(); i++) {
      if (visited_vert_[i]) unvisited = i;
    }
    if (DFS_stack.empty() && unvisited && step <= res.size()) {
      DFS_stack.push(res[static_cast<int>(res.size() - step)]);
      step++;
    }
  }
  return res;
}
