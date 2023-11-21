#ifndef SRC_GRAPH_ALGORITHMS_S21_SOLVE_TRAVELING_SALESMAN_H_
#define SRC_GRAPH_ALGORITHMS_S21_SOLVE_TRAVELING_SALESMAN_H_

#include <algorithm>
#include <cmath>
#include <exception>
#include <map>
#include <numeric>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "s21_breadth_search.h"
#include "s21_graph.h"
#include "s21_tsm_result.h"

namespace s21 {

class SolveTravelingSalesman {
 public:
  enum TypeAnts { SIMPLE_ANT, ELITE_ANT };

  SolveTravelingSalesman() = default;
  SolveTravelingSalesman(Graph& graph);
  ~SolveTravelingSalesman();

  TsmResult GetSolveTravelingSalesmanProblemResult();

 private:
  static constexpr double kAlpha = 2.0;
  static constexpr double kBeta = 3.7;
  static constexpr double kGamma = 0.00142;
  static constexpr double kP = 0.955;
  static constexpr double kStartTau = 1.0;
  static constexpr double kEliteToSimple = 0.25;
  static constexpr double kElitePheromones = 1.3;

  std::default_random_engine& RandomEngine();
  void Init();
  void InitPheromones();
  void RunOneIteration();
  void DefineTypeAnt(TypeAnts& type_ant, int& simple_ants, int& elite_ants);
  TsmResult RunOneAnt(int vertex_from, TypeAnts& type_ant);
  void InitInverseLengths();
  TypeAnts RandomTypeAnt();
  void GenerateUnvisitedVertices(
      std::unordered_map<int, double>& vertices_probabilities_unvisited,
      const int& start_vertex);
  void GenerateProbabilities(
      std::unordered_map<int, double>& probabilities_unvisited,
      std::vector<int>& path);
  int GenerateWheelFortune(
      std::unordered_map<int, double>& probabilities_unvisited);
  int SpinWheelFortune(const std::map<double, int>& probabilities_vertices);
  double FindMaxPheromone(const int& vertex);
  void LocalUpdatePheromone(const int& vertex_from, const int& vertex_to,
                            TypeAnts& type_ant);
  void RunSmallDistance(
      TsmResult& result,
      std::unordered_map<int, double>& vertices_probabilities_unvisited,
      int& vertex_from, TypeAnts& type_ant);
  double FindCommonProbability(
      const std::unordered_map<int, double>& vertices_probabilities_unvisited);
  void ReducePheromones();
  void IncreaseVerticesInResult();
  bool CheckSinglePath(const int& last_vertex, int& single_vertex);

  Graph graph_;
  int count_vertices_;
  int simple_ants_;
  int elite_ants_;
  S21Matrix pheromones_;
  S21Matrix inverse_length_;
  int iterations_;
  std::unordered_set<Graph::Edge> best_edges_;
  TsmResult best_result_;
};

}  // namespace s21

#endif  // SRC_GRAPH_ALGORITHMS_S21_SOLVE_TRAVELING_SALESMAN_H_
