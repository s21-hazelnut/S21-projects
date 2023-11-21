#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

#include "Controller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  Ui::MainWindow *ui;
  double x_begin, x_end, step, X, y_min, y_max, Y;
  int N;
  QVector<double> x, y;

 private slots:
  void click_numbers();
  void on_pushButton_dot_clicked();
  void on_pushButton_sbros_clicked();
  void on_pushButton_rezult_clicked();
  void on_pushButton_graph_clicked();
  void on_pushButton_credit_clicked();
};
#endif  // MAINWINDOW_H
