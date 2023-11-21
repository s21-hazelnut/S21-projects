#include "s21_floyd_warshall.h"

s21::FloydWarshall::FloydWarshall(Graph& graph) { InitMatrix(graph); }

s21::FloydWarshall::~FloydWarshall() {}

S21Matrix s21::FloydWarshall::GetShortestPathsBetweenAllVertices() {
  for (int k = 0; k < weights_.GetRows(); ++k) {
    for (int i = 0; i < weights_.GetRows(); ++i) {
      if (i == k) continue;
      for (int j = 0; j < weights_.GetCols(); ++j) {
        if (i == j || j == k || weights_(i, k) == INT32_MAX ||
            weights_(k, j) == INT32_MAX)
          continue;
        weights_(i, j) =
            std::min(weights_(i, j), weights_(i, k) + weights_(k, j));
      }
    }
  }
  DeleteInfInMatrix();
  return weights_;
}

void s21::FloydWarshall::InitMatrix(Graph& graph) {
  weights_ = std::move(graph.GetMatrix());
  for (int i = 0; i < weights_.GetRows(); ++i) {
    for (int j = 0; j < weights_.GetCols(); ++j) {
      if (i == j) continue;
      if (weights_(i, j) == 0) weights_(i, j) = INT32_MAX;
    }
  }
}

void s21::FloydWarshall::DeleteInfInMatrix() {
  for (int i = 0; i < weights_.GetRows(); ++i) {
    for (int j = 0; j < weights_.GetCols(); ++j) {
      if (i == j) continue;
      if (weights_(i, j) == INT32_MAX) weights_(i, j) = 0;
    }
  }
}
