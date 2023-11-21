#ifndef SRC_GRAPH_ALGORITHMS_S21_TSM_RESULT_H_
#define SRC_GRAPH_ALGORITHMS_S21_TSM_RESULT_H_

#include <numeric>
#include <vector>

#include "s21_graph.h"

namespace s21 {

struct TsmResult {
  TsmResult() = default;
  TsmResult(Graph& graph, int vertex) : TsmResult(graph, vertex, 0.0) {}

  TsmResult(Graph& graph, int vertex, double this_distance)
      : distance{this_distance}, graph_{graph} {
    path.push_back(vertex);
  }

  TsmResult(Graph& graph, std::vector<int>& this_path, double& distance)
      : path{this_path}, distance{distance}, graph_{graph} {}

  TsmResult(const TsmResult& other)
      : path{other.path}, distance{other.distance}, graph_{other.graph_} {}

  TsmResult& operator=(const TsmResult& other) {
    path = other.path;
    distance = other.distance;
    graph_ = other.graph_;
    return *this;
  }

  void operator=(TsmResult&& other) {
    std::swap(path, other.path);
    std::swap(distance, other.distance);
    std::swap(graph_, other.graph_);
  }

  bool operator<(const TsmResult& other) const {
    return distance < other.distance;
  }

  std::vector<int> path;
  double distance;

  void add(const int& vertex) {
    double weight = graph_.Weight(path.back() + 1, vertex + 1);
    if (!weight) weight = std::numeric_limits<double>::infinity();
    distance += weight;
    path.push_back(vertex);
  }

 private:
  Graph graph_;
};

}  // namespace s21

#endif  // SRC_GRAPH_ALGORITHMS_S21_TSM_RESULT_H_
