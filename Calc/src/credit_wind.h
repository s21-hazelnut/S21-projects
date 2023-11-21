#ifndef CREDIT_WIND_H
#define CREDIT_WIND_H

#include <QDialog>

#include "Controller.h"

namespace Ui {
class credit_wind;
}

class credit_wind : public QDialog {
  Q_OBJECT

 public:
  explicit credit_wind(QWidget *parent = nullptr);
  ~credit_wind();

 private slots:
  void on_pushButton_clicked();

  void on_radioButton_ann_clicked(bool checked);

  void on_radioButton_dif_clicked(bool checked);

 private:
  Ui::credit_wind *ui;
};

#endif  // CREDIT_WIND_H
