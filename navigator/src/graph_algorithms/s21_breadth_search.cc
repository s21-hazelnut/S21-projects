#include "s21_breadth_search.h"

s21::BreadthSearch::BreadthSearch(Graph& graph) : graph_{graph} {}

s21::BreadthSearch::~BreadthSearch() {}

void s21::BreadthSearch::InitVisited() {
  visited_vert_ = std::vector<bool>(graph_.GetCountOfVertexes());
}

std::vector<int> s21::BreadthSearch::GetBreadthSearchResult(
    const int& start_vertex) {
  std::vector<int> path;
  if (start_vertex < 1 || start_vertex > graph_.GetCountOfVertexes())
    return path;

  InitVisited();
  visited_vert_[start_vertex] = true;
  s21::queue<int> BFS_queue;
  BFS_queue.push(start_vertex);
  path.push_back(start_vertex);
  while (!BFS_queue.empty()) {
    int temp = BFS_queue.pop();
    for (int i = 1; i <= graph_.GetCountOfVertexes(); i++) {
      if (graph_.Weight(temp, i) && !visited_vert_[i]) {
        visited_vert_[i] = true;
        path.push_back(i);
        BFS_queue.push(i);
      }
    }
  }
  return path;
}
