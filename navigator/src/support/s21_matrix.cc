#include "s21_matrix.h"

S21Matrix::S21Matrix() : S21Matrix(1, 1) { CreateMatrix(); }

S21Matrix::~S21Matrix() {
  if (matrix_) DelMatrix();
}

S21Matrix::S21Matrix(int rows, int cols) {
  if (rows <= 0 || cols <= 0)
    throw std::invalid_argument("Invalid rows/cols in Matrix!\n");
  cols_ = cols;
  rows_ = rows;
  CreateMatrix();
}

S21Matrix::S21Matrix(int rows, int cols,
                     std::initializer_list<double> const& items)
    : S21Matrix(rows, cols) {
  auto it = items.begin();
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] = *it;
      ++it;
    }
  }
}

S21Matrix::S21Matrix(const S21Matrix& other)
    : S21Matrix::S21Matrix(other.rows_, other.cols_) {
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] = other.matrix_[i][j];
    }
  }
}

S21Matrix::S21Matrix(S21Matrix&& other) {
  if (this == &other) return;
  std::swap(rows_, other.rows_);
  std::swap(cols_, other.cols_);
  std::swap(matrix_, other.matrix_);
}

void S21Matrix::SetZero(int row) {
  for (int i = 0; i < cols_; i++) {
    matrix_[row][i] = 0;
  }
}

void S21Matrix::CreateMatrix() {
  matrix_ = new double*[rows_];
  if (matrix_) {
    for (int i = 0; i < rows_; i++) {
      matrix_[i] = new double[cols_];
      SetZero(i);
    }
  }
}

void S21Matrix::DelMatrix() {
  if (matrix_) {
    for (int i = 0; i < rows_; i++) {
      delete[] matrix_[i];
    }
    delete[] matrix_;
    matrix_ = nullptr;
  }
}

void S21Matrix::SumMatrix(const S21Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_)
    throw std::invalid_argument("Matrix is in trouble");
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] += other.matrix_[i][j];
    }
  }
}

bool S21Matrix::EqMatrix(const S21Matrix& other) const {
  bool equal = (rows_ == other.rows_) && (cols_ == other.cols_);
  for (int i = 0; equal && i < rows_; i++)
    for (int j = 0; equal && j < cols_; j++)
      equal = equal && (fabs(matrix_[i][j] - other.matrix_[i][j]) < EPS);
  return equal;
}

void S21Matrix::SubMatrix(const S21Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_)
    throw std::invalid_argument("Matrix is in trouble");
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] -= other.matrix_[i][j];
    }
  }
}

void S21Matrix::MulNumber(const double num) {
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] *= num;
    }
  }
}

void S21Matrix::MulMatrix(const S21Matrix& other) {
  S21Matrix newmatrix(rows_, other.cols_);
  if (cols_ != other.rows_) throw std::invalid_argument("Matrix is in trouble");
  for (int i = 0; i < newmatrix.rows_; i++) {
    for (int j = 0; j < newmatrix.cols_; j++) {
      for (int k = 0; k < cols_; k++) {
        newmatrix.matrix_[i][j] += matrix_[i][k] * other.matrix_[k][j];
      }
    }
  }
  SetCols(other.cols_);
  CopyMatrix(*this, newmatrix);
}

S21Matrix S21Matrix::Transpose() {
  S21Matrix newmatrix(cols_, rows_);
  for (int i = 0; i < newmatrix.rows_; i++) {
    for (int j = 0; j < newmatrix.cols_; j++) {
      newmatrix.matrix_[i][j] = matrix_[j][i];
    }
  }
  return newmatrix;
}

S21Matrix S21Matrix::CalcComplements() {
  S21Matrix newmatrix(rows_, cols_);
  if (cols_ != rows_) throw std::invalid_argument("Matrix is in trouble");
  for (int i = 0; i < newmatrix.rows_; i++) {
    for (int j = 0; j < newmatrix.cols_; j++) {
      S21Matrix tmp = MinorMatrix(i, j);
      double det = tmp.Determinant();
      newmatrix.matrix_[i][j] = det * (((i + j) % 2) ? -1.0 : 1.0);
    }
  }
  return newmatrix;
}

double S21Matrix::Determinant() {
  double result = 0.0;
  if (cols_ != rows_) throw std::invalid_argument("Matrix is in trouble");
  if (rows_ < 3) {
    if (rows_ == 1) result = matrix_[0][0];
    if (rows_ == 2)
      result = matrix_[0][0] * matrix_[1][1] - matrix_[0][1] * matrix_[1][0];
  }
  if (rows_ >= 3) {
    for (int i = 0; i < cols_; i++) {
      S21Matrix temp = MinorMatrix(0, i);
      double det = temp.Determinant();
      result += (pow(-1, i)) * matrix_[0][i] * det;
    }
  }
  return result;
}

S21Matrix S21Matrix::MinorMatrix(int row, int column) {
  S21Matrix dest((rows_ - 1), (cols_ - 1));
  for (int i = 0; i < dest.rows_; i++) {
    if (i < row) {
      for (int j = 0; j < dest.cols_; j++) {
        if (j < column) dest.matrix_[i][j] = matrix_[i][j];
        if (j > column - 1) dest.matrix_[i][j] = matrix_[i][j + 1];
      }
    }
    if (i > row - 1) {
      for (int j = 0; j < dest.cols_; j++) {
        if (j < column) dest.matrix_[i][j] = matrix_[i + 1][j];
        if (j > column - 1) dest.matrix_[i][j] = matrix_[i + 1][j + 1];
      }
    }
  }
  return dest;
}

S21Matrix S21Matrix::InverseMatrix() {
  S21Matrix newmatrix;
  double det = Determinant();
  if (fabs(det) < EPS) det = 0.0;
  if (!det || cols_ != rows_)
    throw std::invalid_argument("Determinant = 0 :( ");
  S21Matrix temp = CalcComplements();
  S21Matrix tmp = temp.Transpose();
  tmp.MulNumber(1.0 / det);
  CopyMatrix(newmatrix, tmp);
  return newmatrix;
}

bool S21Matrix::operator==(const S21Matrix& other) const {
  return this->EqMatrix(other);
}

S21Matrix& S21Matrix::operator=(const S21Matrix& other) {
  if (this != &other) {
    CopyMatrix(*this, other);
  }
  return *this;
}

S21Matrix S21Matrix::operator+(const S21Matrix& other) {
  S21Matrix newmatrix(*this);
  newmatrix.SumMatrix(other);
  return newmatrix;
}

S21Matrix S21Matrix::operator-(const S21Matrix& other) {
  S21Matrix newmatrix(*this);
  newmatrix.SubMatrix(other);
  return newmatrix;
}

S21Matrix S21Matrix::operator*(const S21Matrix& other) {
  S21Matrix newmatrix(*this);
  newmatrix.MulMatrix(other);
  return newmatrix;
}

S21Matrix operator*(const double value, const S21Matrix& other) {
  S21Matrix newmatrix(other.GetRows(), other.GetCols());
  newmatrix = other;
  newmatrix.MulNumber(value);
  return newmatrix;
}

S21Matrix operator*(const S21Matrix& other, const double value) {
  S21Matrix newmatrix(other.GetRows(), other.GetCols());
  newmatrix = operator*(value, other);
  return newmatrix;
}

void S21Matrix::operator+=(const S21Matrix& other) { this->SumMatrix(other); }

void S21Matrix::operator-=(const S21Matrix& other) { this->SubMatrix(other); }

void S21Matrix::operator*=(const S21Matrix& other) { this->MulMatrix(other); }

void S21Matrix::operator*=(const double num) { this->MulNumber(num); }

double& S21Matrix::operator()(int i, int j) {
  if (i < 0 || i >= rows_ || j < 0 || j >= cols_)
    throw std::invalid_argument("Index outside matrix dimensions");
  return matrix_[i][j];
}

double S21Matrix::GetElement(int i, int j) {
  if (i < 0 || i >= rows_ || j < 0 || j >= cols_)
    throw std::invalid_argument("Index outside matrix dimensions");
  return matrix_[i][j];
}

void S21Matrix::SetElement(int i, int j, double value) {
  if (i < 0 || i >= rows_ || j < 0 || j >= cols_)
    throw std::invalid_argument("Index outside matrix dimensions");

  matrix_[i][j] = value;
}

int S21Matrix::GetRows() const { return rows_; }

int S21Matrix::GetCols() const { return cols_; }

void S21Matrix::SetRows(int new_rows) {
  if (new_rows <= 0) {
    throw std::invalid_argument("number of rows cannot be less than 1");
  }
  int tmp_rows = (rows_ >= new_rows) ? new_rows : rows_;
  S21Matrix temp(new_rows, cols_);
  for (int i = 0; i < tmp_rows; i++) {
    for (int j = 0; j < cols_; j++) {
      temp.matrix_[i][j] = matrix_[i][j];
    }
  }
  DelMatrix();
  rows_ = new_rows;
  CopyMatrix(*this, temp);
}

void S21Matrix::SetCols(int new_cols) {
  if (new_cols <= 0) {
    throw std::invalid_argument("number of columns cannot be less than 1");
  }
  int tmp_cols = (cols_ >= new_cols) ? new_cols : cols_;
  S21Matrix temp(rows_, new_cols);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < tmp_cols; j++) {
      temp.matrix_[i][j] = matrix_[i][j];
    }
  }
  DelMatrix();
  cols_ = new_cols;
  CopyMatrix(*this, temp);
}

void S21Matrix::CopyMatrix(S21Matrix& A, const S21Matrix& B) {
  if (A.matrix_) {
    for (int i = 0; i < A.rows_; i++) delete[] A.matrix_[i];
    delete[] A.matrix_;
    A.matrix_ = nullptr;
  }
  A.rows_ = B.rows_;
  A.cols_ = B.cols_;
  A.CreateMatrix();
  for (int i = 0; i < A.rows_; i += 1) {
    for (int j = 0; j < A.cols_; j += 1) {
      A.matrix_[i][j] = B.matrix_[i][j];
    }
  }
}

void S21Matrix::Fill(double value) {
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] = value;
    }
  }
}

void S21Matrix::Print() {
  using namespace std;
  cout << setw(4) << "";
  for (int j = 0; j < cols_; ++j) {
    cout << setw(4) << j + 1;
  }
  cout << "\n";
  for (int i = 0; i < rows_; ++i) {
    cout << setw(4) << i + 1;
    for (int j = 0; j < cols_; ++j) {
      cout << setw(4) << matrix_[i][j];
    }
    cout << "\n";
  }
}