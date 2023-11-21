#ifndef SRC_SUPPORT_S21_MATRIX_H_
#define SRC_SUPPORT_S21_MATRIX_H_

#include <cmath>
#include <iomanip>
#include <iostream>

class S21Matrix {
 public:
  S21Matrix();   // Default constructor
  ~S21Matrix();  // Destructor
  S21Matrix(int rows, int cols);  // Параметризированный конструктор с
                                  // количеством строк и столбцов
  S21Matrix(int rows, int cols, std::initializer_list<double> const& items);
  S21Matrix(const S21Matrix& other);  // Конструктор копирования
  S21Matrix(S21Matrix&& other);  // Конструктор переноса

  class Row {
    friend class S21Matrix;

   public:
    double& operator[](int col) { return parent_.matrix_[row][col]; }

   private:
    Row(S21Matrix& parent, int row_) : parent_(parent), row(row_) {}
    S21Matrix& parent_;
    int row;
  };

  void Fill(double value);
  void SumMatrix(const S21Matrix& other);  // суммирование матриц
  bool EqMatrix(const S21Matrix& other) const;  // равенство матриц
  void SubMatrix(const S21Matrix& other);  // вычитание матриц
  void MulNumber(const double num);  // умножение матрицы на число
  void MulMatrix(const S21Matrix& other);  // умножение матрицы на матрицу
  S21Matrix Transpose();        // транспонирование матриц
  S21Matrix CalcComplements();  // Вычисляет матрицу алгебраических дополнений
                                // текущей матрицы и возвращает ее
  double Determinant();       // Детерминант матриц
  S21Matrix InverseMatrix();  // обратная матрица
  void Print();

  Row operator[](int row) { return Row(*this, row); }
  bool operator==(const S21Matrix& other) const;
  S21Matrix& operator=(const S21Matrix& other);
  S21Matrix operator+(const S21Matrix& other);
  S21Matrix operator-(const S21Matrix& other);
  S21Matrix operator*(const S21Matrix& other);
  void operator+=(const S21Matrix& other);
  void operator-=(const S21Matrix& other);
  void operator*=(const S21Matrix& other);
  void operator*=(const double num);
  double& operator()(int i, int j);
  friend S21Matrix operator*(const double value, const S21Matrix& other);
  friend S21Matrix operator*(const S21Matrix& other, const double value);

  // Для тестов
  const double EPS = 1E-8;
  double GetElement(int i, int j);
  void SetElement(int i, int j, double value);
  int GetRows() const;
  int GetCols() const;
  void SetCols(int new_cols);
  void SetRows(int new_rows);

 private:
  void CreateMatrix();
  void DelMatrix();
  S21Matrix MinorMatrix(int row, int column);
  void CopyMatrix(S21Matrix& A, const S21Matrix& B);
  void SetZero(int row);

  int rows_, cols_;
  double** matrix_;
};

#endif  // SRC_SUPPORT_S21_MATRIX_H_
