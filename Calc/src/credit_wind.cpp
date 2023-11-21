#include "credit_wind.h"

#include "ui_credit_wind.h"

int type;
credit_wind::credit_wind(QWidget *parent)
    : QDialog(parent), ui(new Ui::credit_wind) {
  ui->setupUi(this);
}

credit_wind::~credit_wind() { delete ui; }

void credit_wind::on_radioButton_ann_clicked(bool checked) {
  if (checked) {
    type = 1;
  }
}

void credit_wind::on_radioButton_dif_clicked(bool checked) {
  if (checked) {
    type = 2;
  }
}

void credit_wind::on_pushButton_clicked() {
  double summ, month, proc, over, all;
  int term;
  summ = ui->textEdit_summ->toPlainText().toDouble();
  term = ui->textEdit_term->toPlainText().toInt();
  proc = ui->textEdit_proc->toPlainText().toDouble();
  s21::Controller RES;
  RES.Credit(summ, term, proc, type, &month, &over, &all);

  QString toStr_mohth = QString::number(month, 'g', 7);
  ui->label_month->setText(toStr_mohth);

  QString toStr_over = QString::number(over, 'g', 7);
  ui->label_over->setText(toStr_over);

  QString toStr_all = QString::number(all, 'g', 7);
  ui->label_all->setText(toStr_all);
}
