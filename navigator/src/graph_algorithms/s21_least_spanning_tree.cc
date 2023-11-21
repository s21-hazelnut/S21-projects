#include "s21_least_spanning_tree.h"

s21::SpanningTree::SpanningTree(Graph &graph)
    : graph_{graph}, vertices_count_{graph_.GetCountOfVertexes()} {}

s21::SpanningTree::~SpanningTree() {}

void s21::SpanningTree::Init() {
  spanning_tree_ = std::move(S21Matrix(vertices_count_, vertices_count_));
  unvisited_.clear();
  for (int v = 1; v <= vertices_count_; ++v) {
    unvisited_.insert(v);
  }
  visited_.clear();
}

void s21::SpanningTree::FromVectorToMatrix() {
  for (const auto &edge : tree_edges_) {
    spanning_tree_.SetElement(edge.vertex_from - 1, edge.vertex_to - 1,
                              edge.weight);
    spanning_tree_.SetElement(edge.vertex_to - 1, edge.vertex_from - 1,
                              edge.weight);
  }
}

void s21::SpanningTree::UnvisitToVisit(int vertex) {
  visited_.insert(vertex);
  unvisited_.erase(vertex);
}

S21Matrix s21::SpanningTree::GetLeastSpanningTree() {
  std::uniform_int_distribution dist(1, vertices_count_ + 1);
  Init();
  int start_vertex = dist(RandomEngine());
  UnvisitToVisit(start_vertex);

  while (!unvisited_.empty()) {
    Edge edge{10000, 10000, INT32_MAX};
    for (const auto &from : visited_) {
      for (int to = 1; to <= vertices_count_; ++to) {
        bool is_unvisited_vertex = unvisited_.find(to) != unvisited_.end();
        bool edge_is_exists = graph_.Weight(from, to);
        if (edge_is_exists && is_unvisited_vertex) {
          int existed_weight = graph_.Weight(from, to);
          if (edge.weight > existed_weight) edge = {from, to, existed_weight};
        }
      }
    }
    if (edge.weight != INT32_MAX) {
      tree_edges_.emplace_back(edge);
      UnvisitToVisit(edge.vertex_to);
    }
  }
  FromVectorToMatrix();
  return spanning_tree_;
}

std::default_random_engine &s21::SpanningTree::RandomEngine() {
  static std::random_device rd;
  static std::default_random_engine gen(rd());
  return gen;
}
