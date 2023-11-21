#include "s21_console_interface.h"

s21::ConsoleInterface::ConsoleInterface()
    : graph_{std::make_shared<s21::Graph>()},
      algorithm_{std::make_shared<GraphAlgorithms>()},
      graph_load_success_{false} {}

s21::ConsoleInterface::~ConsoleInterface() {}

void s21::ConsoleInterface::Start() {
  Clear();
  LoadGraph();
  if (!graph_load_success_) return;
  MSleep(800);
  while (true) {
    Clear();
    int choise = Menu();
    Clear();
    if (func_.find(choise) == func_.end() || !func_[choise]) break;
    func_[choise]();
    if (!graph_load_success_) return;
    if (!Continue()) break;
  }
  Clear();
}

void s21::ConsoleInterface::LoadGraph() {
  using namespace std;
  graph_load_success_ = false;
  while (true) {
    try {
      ColorPrint("Введи номер графа в папке data", "blue");
      string filename;
      cin >> filename;
      filename = "data/" + filename + ".txt";
      graph_->LoadGraphFromFile(filename);
      ColorPrint("Граф загружен!", "green");
      graph_load_success_ = true;
      break;
    } catch (ios_base::failure& e) {
      cout << e.what() << endl;
      ColorPrint("Повторить загрузку графа? y/n", "green");
      char answer;
      if (!Cin(answer) || answer != 'y') break;
    }
  }
}

void s21::ConsoleInterface::ColorPrint(const std::string& str,
                                       const std::string& color) {
  std::cout << "\e[1;" << color_.at(color) << "m" << str << "\e[0m\n";
}

int s21::ConsoleInterface::Menu() {
  using namespace std;
  ColorPrint("Какое действие выполнить?", "red");
  cout << "1  -  Обход графа в ширину\n"
       << "2  -  Обход графа в глубину\n"
       << "3  -  Поиск кратчайшего пути между произвольными двумя вершинами\n"
       << "4  -  Поиск кратчайших путей между всеми парами вершин в графе\n"
       << "5  -  Поиск минимального остовного дерева в графе\n"
       << "6  -  Решение задачи коммивояжера\n"
       << "7  -  Выгрузка графа в формате dot\n"
       << "8  -  Изменить граф\n"
       << "9  -  Выход из программы\n";
  int choice;
  int last_num = static_cast<int>(func_.size());
  while (!Cin(choice) || choice < 1 || choice > last_num) {
    ColorPrint("Введены некорректные данные!", "red");
    ColorPrint("Введи число от 1 до " + std::to_string(last_num), "green");
  }
  return choice;
}

void s21::ConsoleInterface::BreadthSearch() {
  int start_vertex = 0;
  if (!GetVertexFromConsole(start_vertex, "Введи номер начальной вершины"))
    return;

  std::vector<int> res = algorithm_->BreadthFirstSearch(*graph_, start_vertex);
  ColorPrint("Результат обхода в ширину с начальной вершиной " +
                 std::to_string(start_vertex),
             "blue");
  for (size_t i = 0; i < res.size(); ++i) {
    std::cout << res[i];
    if (i < res.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
}

void s21::ConsoleInterface::DepthSearch() {
  int start_vertex = 0;
  if (!GetVertexFromConsole(start_vertex, "Введи номер начальной вершины"))
    return;

  std::vector<int> res = algorithm_->DepthFirstSearch(*graph_, start_vertex);
  ColorPrint("Результат обхода в глубину с начальной вершиной " +
                 std::to_string(start_vertex),
             "blue");
  for (size_t i = 0; i < res.size(); ++i) {
    std::cout << res[i];
    if (i < res.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
}

void s21::ConsoleInterface::ShortestPathBetweenTwoVertices() {
  int start_vertex = 0, end_vertex = 0;
  if (!GetVertexFromConsole(start_vertex, "Введи номер начальной вершины") ||
      !GetVertexFromConsole(end_vertex, "Введи номер конечной вершины"))
    return;
  int min_path = algorithm_->GetShortestPathBetweenVertices(
      *graph_, start_vertex, end_vertex);
  std::cout << "Минимальный путь между вершинами " << start_vertex << " и "
            << end_vertex << " = " << min_path << "\n";
}

void s21::ConsoleInterface::ShortestPathBetweenAll() {
  ColorPrint("Матрица кратчайших путей между вершинами", "blue");
  S21Matrix res = algorithm_->GetShortestPathsBetweenAllVertices(*graph_);
  res.Print();
}

void s21::ConsoleInterface::SpanningTree() {
  Clear();
  S21Matrix res = algorithm_->LeastSpanningTree(*graph_);
  using namespace std;
  ColorPrint("Минимальное остовное дерево:", "blue");

  int max_j = 0, max_length = 0;
  for (int i = 0; i < res.GetRows(); ++i) {
    for (int j = i; j < res.GetCols(); ++j) {
      max_j = std::max(max_j, j + 1);
      max_length = std::max(max_length, static_cast<int>(res.GetElement(i, j)));
    }
  }
  int width_j = static_cast<int>(std::ceil(log10(max_j)));
  int width_length = static_cast<int>(std::ceil(log10(max_length)));

  cout << setw(width_j) << " ";
  for (int i = 0; i < res.GetRows(); ++i) {
    cout << setw(max(width_j, width_length) + 1) << i + 1;
  }
  cout << "\n";
  for (int j = 0; j < res.GetRows(); ++j) {
    cout << setw(width_j) << j + 1;
    for (int i = 0; i < res.GetCols(); ++i) {
      cout << setw(max(width_j, width_length) + 1) << res.GetElement(j, i);
    }
    cout << endl;
  }

  for (int i = 0; i < res.GetRows(); ++i) {
    for (int j = i; j < res.GetCols(); ++j) {
      if (res.GetElement(i, j)) {
        cout << setw(width_j) << i + 1 << " -- " << setw(width_j) << j + 1
             << " длина " << setw(width_length)
             << static_cast<int>(res.GetElement(i, j)) << endl;
      }
    }
  }
}

void s21::ConsoleInterface::TravelingSalesmanProblem() {
  s21::TsmResult result(
      std::move(algorithm_->SolveTravelingSalesmanProblem(*graph_)));
  Clear();
  ColorPrint("Решение задачи коммивояжера:", "blue");

  if (result.distance != std::numeric_limits<double>::infinity() &&
      result.path.size() > 1) {
    std::cout << result.path[0];
    for (size_t i = 1; i < result.path.size(); ++i) {
      std::cout << " -> " << result.path[i];
    }
    std::cout << "\nОбщая протяженность пути: ";
    ColorPrint(std::to_string(static_cast<int>(result.distance)), "blue");
  } else {
    std::cout << "\nГраф не содержит эйлерову цепь.\n";
  }
}

void s21::ConsoleInterface::Clear() {
#if defined _WIN32
  system("cls");
#elif defined(__LINUX__) || defined(__gnu_linux__) || defined(__linux__) || \
    defined(__APPLE__)
  system("clear");
#endif
}

void s21::ConsoleInterface::MSleep(const int& msec) {
  std::this_thread::sleep_for(std::chrono::milliseconds(msec));
}

bool s21::ConsoleInterface::Continue() {
  MSleep(800);
  ColorPrint("Продолжим? y/n", "green");
  char c;
  while (!Cin(c) || (c != 'y' && c != 'n' && c != 'Y' && c != 'N')) {
    ColorPrint("Введи 'y' или 'n'", "green");
  }
  return c == 'y' || c == 'Y';
}

bool s21::ConsoleInterface::GetVertexFromConsole(int& vertex,
                                                 const std::string& msg) {
  bool res = true;
  ColorPrint(msg, "green");
  while (res && !CheckInputForVertex(vertex)) {
    ColorPrint("Номер вершины может быть в интервале от 1 до " +
                   std::to_string(graph_->GetCountOfVertexes()),
               "red");
    ColorPrint("Попробуем ещё раз? y/n", "green");
    char answer;
    if (!Cin(answer) || (answer != 'y' && answer != 'Y')) {
      res = false;
    } else {
      Clear();
      ColorPrint(msg, "green");
    }
  }
  Clear();
  return res;
}

bool s21::ConsoleInterface::CheckInputForVertex(int& vertex) {
  if (Cin(vertex) && vertex >= 1 && vertex <= graph_->GetCountOfVertexes())
    return true;
  return false;
}

void s21::ConsoleInterface::Save() {
  graph_->ExportGraphToDot("build/todot.dot");
  ColorPrint("Сохранено в папку build. Название файла todot.dot", "green");
}

void s21::ConsoleInterface::ClearBuffer() {
  std::cin.clear();
  std::cin.ignore(10000, '\n');
}

bool s21::ConsoleInterface::Cin(char& c) {
  ClearBuffer();
  return static_cast<bool>(std::cin >> c);
}

bool s21::ConsoleInterface::Cin(int& num) {
  ClearBuffer();
  return static_cast<bool>(std::cin >> num);
}
