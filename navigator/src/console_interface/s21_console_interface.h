#ifndef SRC_CONSOLE_INTERFACE_S21_CONSOLE_INTERFACE_H_
#define SRC_CONSOLE_INTERFACE_S21_CONSOLE_INTERFACE_H_

#include <chrono>
#include <cmath>
#include <functional>
#include <memory>
#include <numeric>
#include <string>
#include <thread>
#include <unordered_map>

#include "s21_graph.h"
#include "s21_graph_algorithms.h"

namespace s21 {

class ConsoleInterface {
  using Color = std::unordered_map<std::string, int>;
  using Func = std::unordered_map<int, std::function<void()>>;

 public:
  ConsoleInterface();
  ~ConsoleInterface();
  void Start();

 private:
  void LoadGraph();
  int Menu();
  void BreadthSearch();
  void DepthSearch();
  void ShortestPathBetweenTwoVertices();
  void ShortestPathBetweenAll();
  void SpanningTree();
  void TravelingSalesmanProblem();
  void ColorPrint(const std::string& str, const std::string& color);
  void Clear();
  void MSleep(const int& msec);
  bool Continue();
  bool GetVertexFromConsole(int& vertex, const std::string& msg);
  bool CheckInputForVertex(int& vertex);
  void Save();
  void ClearBuffer();
  bool Cin(char& c);
  bool Cin(int& c);

  std::shared_ptr<Graph> graph_;
  std::shared_ptr<GraphAlgorithms> algorithm_;
  bool graph_load_success_;
  Color color_{{"red", 31}, {"green", 32}, {"blue", 34}};
  Func func_{
      {1, std::bind(&ConsoleInterface::BreadthSearch, this)},
      {2, std::bind(&ConsoleInterface::DepthSearch, this)},
      {3, std::bind(&ConsoleInterface::ShortestPathBetweenTwoVertices, this)},
      {4, std::bind(&ConsoleInterface::ShortestPathBetweenAll, this)},
      {5, std::bind(&ConsoleInterface::SpanningTree, this)},
      {6, std::bind(&ConsoleInterface::TravelingSalesmanProblem, this)},
      {7, std::bind(&ConsoleInterface::Save, this)},
      {8, std::bind(&ConsoleInterface::LoadGraph, this)},
      {9, nullptr}};
};

}  // namespace s21

#endif  // SRC_CONSOLE_INTERFACE_S21_CONSOLE_INTERFACE_H_
