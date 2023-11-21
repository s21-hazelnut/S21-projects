#ifndef SRC_GRAPH_ALGORITHMS_S21_DIJKSTRA_H_
#define SRC_GRAPH_ALGORITHMS_S21_DIJKSTRA_H_

#include <queue>
#include <unordered_set>
#include <vector>

#include "s21_graph.h"

namespace s21 {

class Dijkstra {
 public:
  explicit Dijkstra(Graph& graph);
  ~Dijkstra();
  int GetShortestPath(const int& vertex1, const int& vertex2);

 private:
  struct DistIndex {
    DistIndex(int d, int i) : dist{d}, index{i} {};
    int dist;
    int index;
  };

  struct CompDistIndex {
    bool operator()(const DistIndex& a, const DistIndex& b) {
      return a.dist > b.dist;
    }
  };

  void Init();
  void InitVertices(const int& vertex1, const int& vertex2);
  void InitNotVisited();
  void InitDistances();
  void InitMinDistances();
  void ClearPriorityQueue();
  std::vector<int> FindAllNeighbors(const int& vertex);
  void SetDistance(const DistIndex& dist_index, const int& neighbor);
  void AddToMinDist(const DistIndex& dist_index);
  bool ContainsInSet(const int& num) const;
  void CheckVertex(const int& vertex);

  Graph graph_;
  std::priority_queue<DistIndex, std::vector<DistIndex>, CompDistIndex>
      min_dist_;
  std::unordered_set<int> not_visited_;
  std::vector<int> distances_;
  int start_vertex_;
  int end_vertex_;
};

}  // namespace s21

#endif  // SRC_GRAPH_ALGORITHMS_S21_DIJKSTRA_H_