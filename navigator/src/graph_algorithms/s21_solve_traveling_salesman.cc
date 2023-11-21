#include "s21_solve_traveling_salesman.h"

s21::SolveTravelingSalesman::SolveTravelingSalesman(Graph& graph)
    : graph_{graph} {}

s21::SolveTravelingSalesman::~SolveTravelingSalesman() {}

s21::TsmResult
s21::SolveTravelingSalesman::GetSolveTravelingSalesmanProblemResult() {
  Init();
  InitPheromones();
  for (int i = 0; i < iterations_; ++i) {
    RunOneIteration();
    ReducePheromones();
  }
  IncreaseVerticesInResult();
  return best_result_;
}

void s21::SolveTravelingSalesman::IncreaseVerticesInResult() {
  for (int& vertex : best_result_.path) {
    ++vertex;
  }
}

void s21::SolveTravelingSalesman::ReducePheromones() {
  for (int i = 0; i < count_vertices_; ++i) {
    for (int j = 0; j < count_vertices_; ++j) {
      if (i != j) {
        pheromones_(i, j) *= kP;
      }
    }
  }
}

void s21::SolveTravelingSalesman::Init() {
  count_vertices_ = graph_.GetCountOfVertexes();
  elite_ants_ = kEliteToSimple * count_vertices_;
  simple_ants_ = count_vertices_ - elite_ants_;
  iterations_ = 100;
  InitInverseLengths();
  best_result_ =
      std::move(TsmResult(graph_, 0, std::numeric_limits<double>::infinity()));
}

void s21::SolveTravelingSalesman::InitPheromones() {
  pheromones_ = std::move(S21Matrix(count_vertices_, count_vertices_));
  for (int i = 0; i < count_vertices_; ++i) {
    for (int j = 0; j < count_vertices_; ++j) {
      if (i != j) {
        pheromones_(i, j) = kStartTau;
      }
    }
  }
}

void s21::SolveTravelingSalesman::RunOneIteration() {
  int simple_ants = simple_ants_;
  int elite_ants = elite_ants_;
  int ants = simple_ants + elite_ants;

  for (int j = 0; j < ants; ++j) {
    TypeAnts type_ant;
    DefineTypeAnt(type_ant, simple_ants, elite_ants);
    TsmResult result(std::move(RunOneAnt(j, type_ant)));
    if (result < best_result_) best_result_ = std::move(result);
  }
  if (best_result_.path.size() > 2) {
    best_edges_.clear();
    for (size_t i = 1; i < best_result_.path.size(); ++i) {
      best_edges_.emplace(
          Graph::Edge(best_result_.path[i - 1], best_result_.path[i]));
    }
  }
}

void s21::SolveTravelingSalesman::DefineTypeAnt(TypeAnts& type_ant,
                                                int& simple_ants,
                                                int& elite_ants) {
  type_ant = RandomTypeAnt();
  if (!elite_ants || (type_ant == SIMPLE_ANT && simple_ants)) {
    type_ant = SIMPLE_ANT;
    --simple_ants;
  } else {
    type_ant = ELITE_ANT;
    --elite_ants;
  }
}

s21::TsmResult s21::SolveTravelingSalesman::RunOneAnt(int vertex_from,
                                                      TypeAnts& type_ant) {
  int start_vertex = vertex_from;
  TsmResult result(graph_, start_vertex);
  std::unordered_map<int, double> vertices_probabilities_unvisited;
  GenerateUnvisitedVertices(vertices_probabilities_unvisited, start_vertex);
  while (!vertices_probabilities_unvisited.empty()) {
    RunSmallDistance(result, vertices_probabilities_unvisited, vertex_from,
                     type_ant);
  }
  if (inverse_length_(result.path.back(), start_vertex)) {
    result.add(start_vertex);
  } else {
    s21::BreadthSearch algo(graph_);
    std::vector<int> search =
        algo.GetBreadthSearchResult(result.path.back() - 1);
    if (search.empty())
      result.distance += std::numeric_limits<double>::infinity();
    else {
      bool find_start = false;
      for (size_t i = 1; i < search.size(); ++i) {
        result.add(search[i] - 1);
        if (search[i] - 1 == start_vertex) {
          find_start = true;
          break;
        }
      }
      if (!find_start)
        result.distance += std::numeric_limits<double>::infinity();
    }
  }
  return result;
}

void s21::SolveTravelingSalesman::GenerateUnvisitedVertices(
    std::unordered_map<int, double>& vertices_probabilities_unvisited,
    const int& start_vertex) {
  int i = 0;
  std::generate_n(std::inserter(vertices_probabilities_unvisited,
                                vertices_probabilities_unvisited.begin()),
                  count_vertices_, [&i]() -> std::pair<int, double> {
                    return {i++, 0.0};
                  });
  vertices_probabilities_unvisited.erase(start_vertex);
}

void s21::SolveTravelingSalesman::RunSmallDistance(
    TsmResult& result,
    std::unordered_map<int, double>& vertices_probabilities_unvisited,
    int& vertex_from, TypeAnts& type_ant) {
  int vertex_to;

  int single_vertex = 0;
  if (!CheckSinglePath(result.path.back(), single_vertex)) {
    GenerateProbabilities(vertices_probabilities_unvisited, result.path);
    vertex_to = GenerateWheelFortune(vertices_probabilities_unvisited);
  } else {
    vertex_to = single_vertex;
  }
  LocalUpdatePheromone(vertex_from, vertex_to, type_ant);
  result.add(vertex_to);
  vertices_probabilities_unvisited.erase(vertex_to);
  vertex_from = vertex_to;
}

bool s21::SolveTravelingSalesman::CheckSinglePath(const int& last_vertex,
                                                  int& single_vertex) {
  int count_possible_paths = 0;
  for (int vertex = 0; vertex < count_vertices_; ++vertex) {
    if (inverse_length_[last_vertex][vertex]) {
      ++count_possible_paths;
      single_vertex = vertex;
    }
    if (count_possible_paths > 1) return false;
  }
  return true;
}

void s21::SolveTravelingSalesman::GenerateProbabilities(
    std::unordered_map<int, double>& vertices_probabilities_unvisited,
    std::vector<int>& path) {
  for (auto& probability : vertices_probabilities_unvisited) {
    double inverse_length = inverse_length_(path.back(), probability.first);
    double pheromone = pheromones_(path.back(), probability.first);
    probability.second =
        std::pow(pheromone, kAlpha) * std::pow(inverse_length, kBeta);
  }
  double common_probability =
      FindCommonProbability(vertices_probabilities_unvisited);
  if (common_probability == 0.0) return;
  for (auto& probability : vertices_probabilities_unvisited) {
    probability.second /= common_probability;
  }
}

double s21::SolveTravelingSalesman::FindCommonProbability(
    const std::unordered_map<int, double>& vertices_probabilities_unvisited) {
  return std::accumulate(
      std::begin(vertices_probabilities_unvisited),
      std::end(vertices_probabilities_unvisited), 0.0,
      [](double value, const std::unordered_map<int, double>::value_type& p) {
        return value + p.second;
      });
}

int s21::SolveTravelingSalesman::GenerateWheelFortune(
    std::unordered_map<int, double>& vertices_probabilities_unvisited) {
  std::map<double, int> probabilities_vertices;
  double probability_prev = 0.0;
  for (auto& probability : vertices_probabilities_unvisited) {
    if (probability.second) {
      probability_prev += probability.second;
      probabilities_vertices[probability_prev] = probability.first;
    }
  }
  if (probabilities_vertices.empty())
    probabilities_vertices[1.0] =
        vertices_probabilities_unvisited.begin()->first;
  return SpinWheelFortune(probabilities_vertices);
}

int s21::SolveTravelingSalesman::SpinWheelFortune(
    const std::map<double, int>& probabilities_vertices) {
  std::uniform_real_distribution dist(0.0, 1.0);
  double probability = dist(RandomEngine());
  if (!probability) probability = 1.0;
  auto it = probabilities_vertices.lower_bound(probability);
  if (it != probabilities_vertices.end()) return it->second;
  throw(std::runtime_error("Path is not exist from this vertice!\n"));
}

double s21::SolveTravelingSalesman::FindMaxPheromone(const int& vertex) {
  double max_pheromone = 0.0;
  for (int i = 0; i < count_vertices_; ++i) {
    max_pheromone = std::max(max_pheromone, pheromones_[vertex][i]);
  }
  return max_pheromone;
}

void s21::SolveTravelingSalesman::LocalUpdatePheromone(const int& vertex_from,
                                                       const int& vertex_to,
                                                       TypeAnts& type_ant) {
  double delta_pheromone = kGamma * FindMaxPheromone(vertex_to);
  if (type_ant == ELITE_ANT &&
      best_edges_.find(Graph::Edge(vertex_from, vertex_to)) !=
          best_edges_.end())
    delta_pheromone *= kElitePheromones;
  pheromones_[vertex_from][vertex_to] += delta_pheromone;
}

void s21::SolveTravelingSalesman::InitInverseLengths() {
  inverse_length_ = std::move(S21Matrix(count_vertices_, count_vertices_));
  for (int i = 0; i < count_vertices_; ++i) {
    for (int j = 0; j < count_vertices_; ++j) {
      if (!graph_.Weight(i + 1, j + 1))
        inverse_length_[i][j] = 0.0;
      else
        inverse_length_(i, j) = 1.0 / graph_.Weight(i + 1, j + 1);
    }
  }
}

s21::SolveTravelingSalesman::TypeAnts
s21::SolveTravelingSalesman::RandomTypeAnt() {
  std::uniform_int_distribution dist(static_cast<int>(SIMPLE_ANT),
                                     static_cast<int>(ELITE_ANT));
  return static_cast<TypeAnts>(dist(RandomEngine()));
}

std::default_random_engine& s21::SolveTravelingSalesman::RandomEngine() {
  static std::random_device rd;
  static std::default_random_engine gen(rd());
  return gen;
}
