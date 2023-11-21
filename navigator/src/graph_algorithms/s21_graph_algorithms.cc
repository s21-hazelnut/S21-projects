#include "s21_graph_algorithms.h"

s21::GraphAlgorithms::GraphAlgorithms() {}

s21::GraphAlgorithms::~GraphAlgorithms() {}

std::vector<int> s21::GraphAlgorithms::DepthFirstSearch(Graph &graph,
                                                        int start_vertex) {
  s21::DepthSearch algorithm(graph);
  return algorithm.GetDepthSearchResult(start_vertex);
}
std::vector<int> s21::GraphAlgorithms::BreadthFirstSearch(Graph &graph,
                                                          int start_vertex) {
  s21::BreadthSearch algorithm(graph);
  return algorithm.GetBreadthSearchResult(start_vertex);
}

S21Matrix s21::GraphAlgorithms::LeastSpanningTree(Graph &graph) {
  s21::SpanningTree algorithm(graph);
  return algorithm.GetLeastSpanningTree();
}

int s21::GraphAlgorithms::GetShortestPathBetweenVertices(s21::Graph &graph,
                                                         int vertex1,
                                                         int vertex2) {
  s21::Dijkstra algorithm(graph);
  return algorithm.GetShortestPath(vertex1, vertex2);
}

S21Matrix s21::GraphAlgorithms::GetShortestPathsBetweenAllVertices(
    Graph &graph) {
  s21::FloydWarshall algorithm(graph);
  return algorithm.GetShortestPathsBetweenAllVertices();
}

s21::TsmResult s21::GraphAlgorithms::SolveTravelingSalesmanProblem(
    Graph &graph) {
  s21::SolveTravelingSalesman algorithm(graph);
  return algorithm.GetSolveTravelingSalesmanProblemResult();
}