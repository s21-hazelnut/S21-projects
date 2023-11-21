#include "s21_graph.h"

#include <exception>

s21::Graph::Graph() {}

s21::Graph::Graph(const std::string& filename) { LoadGraphFromFile(filename); }

s21::Graph::Graph(int count_vertices) {
  matrix_adjacency_ =
      std::make_shared<S21Matrix>(count_vertices, count_vertices);
}

s21::Graph::~Graph() {}

int s21::Graph::operator()(const int& i, const int& j) { return Weight(i, j); }

void s21::Graph::LoadGraphFromFile(const std::string& filename) {
  std::ifstream fin;
  fin.open(filename);
  if (!fin.is_open())
    throw std::ios_base::failure("Файла " + filename + " не существует!\n");

  int count_vertices;
  if (!(fin >> count_vertices))
    throw std::invalid_argument("Incorrect number of vertices in file!\n");
  matrix_adjacency_ =
      std::make_shared<S21Matrix>(count_vertices, count_vertices);

  for (int i = 0; i < matrix_adjacency_->GetRows(); ++i) {
    for (int j = 0; j < matrix_adjacency_->GetCols(); ++j) {
      int num;
      if (!(fin >> num))
        throw std::invalid_argument("Incorrect edge in file!\n");
      matrix_adjacency_->SetElement(i, j, num);
    }
  }
}

void s21::Graph::ExportGraphToDot(std::string filename) {
  using namespace std;
  ofstream fout;
  fout.open(filename);
  if (!fout.is_open())
    throw ios_base::failure("Cann't create file for export!");

  fout << "digraph {" << endl;

  for (int i = 0; i < matrix_adjacency_->GetRows(); ++i) {
    for (int j = 0; j < matrix_adjacency_->GetCols(); ++j) {
      if (matrix_adjacency_->GetElement(i, j)) {
        fout << "  " << i + 1 << " -> " << j + 1
             << " [label = " << matrix_adjacency_->GetElement(i, j)
             << "; len = " << matrix_adjacency_->GetElement(i, j) << ";];"
             << endl;
      }
    }
  }
  fout << "}";
}

int s21::Graph::GetCountOfVertexes() { return matrix_adjacency_->GetRows(); }

void s21::Graph::AddVertex() {
  matrix_adjacency_->SetRows(matrix_adjacency_->GetRows() + 1);
  matrix_adjacency_->SetCols(matrix_adjacency_->GetCols() + 1);
}

void s21::Graph::SetWeight(int vertex1, int vertex2, int weight) {
  matrix_adjacency_->SetElement(vertex1 - 1, vertex2 - 1, weight);
  matrix_adjacency_->SetElement(vertex2 - 1, vertex1 - 1, weight);
}

int s21::Graph::Weight(int vertex1, int vertex2) {
  return matrix_adjacency_->GetElement(vertex1 - 1, vertex2 - 1);
}

void s21::Graph::Print() { matrix_adjacency_->Print(); }

S21Matrix s21::Graph::GetMatrix() { return *matrix_adjacency_; }
