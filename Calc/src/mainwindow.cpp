#include "mainwindow.h"

#include "credit_wind.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  connect(ui->pushButton_0, SIGNAL(clicked()), this, SLOT(click_numbers()));
  connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(click_numbers()));
  connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(click_numbers()));
  connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(click_numbers()));
  connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(click_numbers()));
  connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(click_numbers()));
  connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(click_numbers()));
  connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(click_numbers()));
  connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(click_numbers()));
  connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(click_numbers()));
  connect(ui->pushButton_plus, SIGNAL(clicked()), this, SLOT(click_numbers()));
  connect(ui->pushButton_minus, SIGNAL(clicked()), this, SLOT(click_numbers()));
  connect(ui->pushButton_mult, SIGNAL(clicked()), this, SLOT(click_numbers()));
  connect(ui->pushButton_del, SIGNAL(clicked()), this, SLOT(click_numbers()));
  connect(ui->pushButton_stepen, SIGNAL(clicked()), this,
          SLOT(click_numbers()));
  connect(ui->pushButton_mod, SIGNAL(clicked()), this, SLOT(click_numbers()));
  connect(ui->pushButton_sqrt, SIGNAL(clicked()), this, SLOT(click_numbers()));
  connect(ui->pushButton_sin, SIGNAL(clicked()), this, SLOT(click_numbers()));
  connect(ui->pushButton_cos, SIGNAL(clicked()), this, SLOT(click_numbers()));
  connect(ui->pushButton_tan, SIGNAL(clicked()), this, SLOT(click_numbers()));
  connect(ui->pushButton_ln, SIGNAL(clicked()), this, SLOT(click_numbers()));
  connect(ui->pushButton_asin, SIGNAL(clicked()), this, SLOT(click_numbers()));
  connect(ui->pushButton_acos, SIGNAL(clicked()), this, SLOT(click_numbers()));
  connect(ui->pushButton_atan, SIGNAL(clicked()), this, SLOT(click_numbers()));
  connect(ui->pushButton_log, SIGNAL(clicked()), this, SLOT(click_numbers()));
  connect(ui->pushButton_opensk, SIGNAL(clicked()), this,
          SLOT(click_numbers()));
  connect(ui->pushButton_closesk, SIGNAL(clicked()), this,
          SLOT(click_numbers()));
  connect(ui->pushButton_X, SIGNAL(clicked()), this,
          SLOT(click_numbers()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::click_numbers() {
  QPushButton *button = (QPushButton *)sender();
  ui->textEdit_input->setText(ui->textEdit_input->toPlainText() +
                              button->text());
}

void MainWindow::on_pushButton_dot_clicked() {
  ui->textEdit_input->setText(ui->textEdit_input->toPlainText() + ".");
}

void MainWindow::on_pushButton_sbros_clicked() {
  ui->textEdit_input->setText("");
}

void MainWindow::on_pushButton_rezult_clicked() {
  double res = 0;
  double x;
  x = ui->textEdit_x->toPlainText().toDouble();
  QString str = ui->textEdit_input->toPlainText();
  std::string a = str.toStdString();
  s21::Controller RES(a);
  res = RES.GetCalcRezult(x);
  QString toStr = QString::number(res, 'g', 7);
  ui->label_out->setText(toStr);
}

void MainWindow::on_pushButton_graph_clicked() {
  ui->widget->clearGraphs();
  ui->widget->replot();
  x.clear();
  y.clear();

  x_begin = ui->textEdit_min->toPlainText().toDouble();
  x_end = ui->textEdit_max->toPlainText().toDouble();
  if (!x_begin || !x_end) {
    x_begin = -5;
    x_end = 5;
  }

  if (x_begin < -1000000) {
    x_begin = -1000000;
  }
  if (x_end > 1000000) {
    x_end = 1000000;
  }

  if (x_begin == x_end) {
    x_begin = -1;
    x_end = 1;
  }

  if (x_begin > x_end) {
    double temp = x_begin;
    x_begin = x_end;
    x_end = temp;
  }
  X = ui->textEdit_x->toPlainText().toDouble();
  QString str = ui->textEdit_input->toPlainText();
  std::string a = str.toStdString();
  step = (x_end - x_begin) / 2000;
  y_min = 100000;
  y_max = -100000;
  double Y;
  s21::Controller RES(a);
  for (X = x_begin; X <= x_end; X += step) {
    Y = RES.GetCalcRezult(X);
    if (Y < y_min) {
      y_min = Y;
    }
    if (Y > y_max) {
      y_max = Y;
    }
  }
  if (y_min < -1000000) {
    y_min = -1000000;
  }
  if (y_max > 1000000) {
    y_max = 1000000;
  }

  ui->widget->xAxis->setRange(x_begin - 4 * step, x_end + 4 * step);
  ui->widget->yAxis->setRange(y_min - (y_max - y_min) * 0.05, y_max + (y_max - y_min) * 0.05);

  for (X = x_begin; X <= x_end; X += step) {
    x.push_back(X);
    y.push_back(RES.GetCalcRezult(X));
  }
  ui->widget->addGraph();
  ui->widget->graph(0)->addData(x, y);
  ui->widget->replot();
  ui->widget->clearGraphs();
}

void MainWindow::on_pushButton_credit_clicked() {
  credit_wind cred;
  cred.setModal(true);
  cred.exec();
}


