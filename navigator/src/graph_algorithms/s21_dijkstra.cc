#include "s21_dijkstra.h"

s21::Dijkstra::Dijkstra(Graph& graph) : graph_{graph} {}

s21::Dijkstra::~Dijkstra() {}

void s21::Dijkstra::Init() {
  InitNotVisited();
  InitDistances();
  InitMinDistances();
}

void s21::Dijkstra::InitVertices(const int& vertex1, const int& vertex2) {
  start_vertex_ = vertex1;
  end_vertex_ = vertex2;
  CheckVertex(start_vertex_);
  CheckVertex(end_vertex_);
}

void s21::Dijkstra::InitNotVisited() {
  not_visited_.clear();
  for (int i = 1; i <= graph_.GetCountOfVertexes(); ++i) {
    not_visited_.insert(i);
  }
}

void s21::Dijkstra::InitDistances() {
  distances_ = std::vector<int>(graph_.GetCountOfVertexes() + 1);
  for (int i = 1; i <= graph_.GetCountOfVertexes(); ++i) {
    distances_[i] = INT32_MAX;
  }
  distances_[start_vertex_] = 0;
}

void s21::Dijkstra::InitMinDistances() {
  ClearPriorityQueue();
  min_dist_.push(DistIndex(0, start_vertex_));
}

void s21::Dijkstra::ClearPriorityQueue() {
  min_dist_ =
      std::priority_queue<DistIndex, std::vector<DistIndex>, CompDistIndex>();
}

std::vector<int> s21::Dijkstra::FindAllNeighbors(const int& vertex) {
  std::vector<int> neighbors;
  for (const int& cur_vertex : not_visited_) {
    if (graph_(vertex, cur_vertex)) {
      neighbors.push_back(cur_vertex);
    }
  }
  return neighbors;
}

void s21::Dijkstra::SetDistance(const DistIndex& dist_index,
                                const int& neighbor) {
  int cur_dist = dist_index.dist + graph_(dist_index.index, neighbor);
  if (cur_dist < distances_[neighbor]) {
    distances_[neighbor] = cur_dist;
    AddToMinDist(DistIndex(cur_dist, neighbor));
  }
}

void s21::Dijkstra::AddToMinDist(const DistIndex& dist_index) {
  min_dist_.push(dist_index);
}

bool s21::Dijkstra::ContainsInSet(const int& num) const {
  return not_visited_.find(num) != not_visited_.end();
}

void s21::Dijkstra::CheckVertex(const int& vertex) {
  if (vertex < 1 || vertex > graph_.GetCountOfVertexes())
    throw std::invalid_argument("Этой вершины нет в графе!");
}

int s21::Dijkstra::GetShortestPath(const int& vertex1, const int& vertex2) {
  InitVertices(vertex1, vertex2);
  Init();
  int min_dist = INT32_MAX;
  while (!min_dist_.empty()) {
    DistIndex cur_dist_index{min_dist_.top()};
    min_dist_.pop();
    if (cur_dist_index.index == end_vertex_) {
      min_dist = cur_dist_index.dist;
      break;
    }
    if (!ContainsInSet(cur_dist_index.index)) continue;
    for (const int& neighbor : FindAllNeighbors(cur_dist_index.index)) {
      SetDistance(cur_dist_index, neighbor);
    }
    not_visited_.erase(cur_dist_index.index);
  }
  return min_dist;
}
