/********************************************************************************
** Form generated from reading UI file 'credit_wind.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CREDIT_WIND_H
#define UI_CREDIT_WIND_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_credit_wind
{
public:
    QLabel *label;
    QTextEdit *textEdit_summ;
    QTextEdit *textEdit_term;
    QLabel *label_term;
    QTextEdit *textEdit_proc;
    QLabel *label_3;
    QRadioButton *radioButton_ann;
    QRadioButton *radioButton_dif;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_month;
    QLabel *label_over;
    QLabel *label_all;
    QPushButton *pushButton;

    void setupUi(QDialog *credit_wind)
    {
        if (credit_wind->objectName().isEmpty())
            credit_wind->setObjectName(QString::fromUtf8("credit_wind"));
        credit_wind->resize(486, 295);
        label = new QLabel(credit_wind);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 200, 20));
        label->setSizeIncrement(QSize(1, 0));
        QFont font;
        font.setPointSize(8);
        label->setFont(font);
        label->setAlignment(Qt::AlignRight|Qt::AlignTop|Qt::AlignTrailing);
        textEdit_summ = new QTextEdit(credit_wind);
        textEdit_summ->setObjectName(QString::fromUtf8("textEdit_summ"));
        textEdit_summ->setGeometry(QRect(10, 30, 200, 40));
        textEdit_term = new QTextEdit(credit_wind);
        textEdit_term->setObjectName(QString::fromUtf8("textEdit_term"));
        textEdit_term->setGeometry(QRect(10, 100, 200, 40));
        label_term = new QLabel(credit_wind);
        label_term->setObjectName(QString::fromUtf8("label_term"));
        label_term->setGeometry(QRect(10, 80, 200, 20));
        label_term->setSizeIncrement(QSize(1, 0));
        label_term->setFont(font);
        label_term->setAlignment(Qt::AlignRight|Qt::AlignTop|Qt::AlignTrailing);
        textEdit_proc = new QTextEdit(credit_wind);
        textEdit_proc->setObjectName(QString::fromUtf8("textEdit_proc"));
        textEdit_proc->setGeometry(QRect(10, 170, 200, 40));
        label_3 = new QLabel(credit_wind);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 150, 200, 20));
        label_3->setSizeIncrement(QSize(1, 0));
        label_3->setFont(font);
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTop|Qt::AlignTrailing);
        radioButton_ann = new QRadioButton(credit_wind);
        radioButton_ann->setObjectName(QString::fromUtf8("radioButton_ann"));
        radioButton_ann->setGeometry(QRect(20, 220, 170, 20));
        QFont font1;
        font1.setPointSize(10);
        radioButton_ann->setFont(font1);
        radioButton_dif = new QRadioButton(credit_wind);
        radioButton_dif->setObjectName(QString::fromUtf8("radioButton_dif"));
        radioButton_dif->setGeometry(QRect(20, 250, 170, 20));
        radioButton_dif->setFont(font1);
        label_4 = new QLabel(credit_wind);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(250, 150, 220, 60));
        label_4->setSizeIncrement(QSize(1, 0));
        label_4->setFont(font);
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTop|Qt::AlignTrailing);
        label_5 = new QLabel(credit_wind);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(250, 10, 220, 60));
        label_5->setSizeIncrement(QSize(1, 0));
        label_5->setFont(font);
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTop|Qt::AlignTrailing);
        label_6 = new QLabel(credit_wind);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(250, 80, 220, 60));
        label_6->setSizeIncrement(QSize(1, 0));
        label_6->setFont(font);
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTop|Qt::AlignTrailing);
        label_month = new QLabel(credit_wind);
        label_month->setObjectName(QString::fromUtf8("label_month"));
        label_month->setGeometry(QRect(250, 30, 220, 40));
        label_month->setSizeIncrement(QSize(1, 0));
        QFont font2;
        font2.setPointSize(24);
        label_month->setFont(font2);
        label_month->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_over = new QLabel(credit_wind);
        label_over->setObjectName(QString::fromUtf8("label_over"));
        label_over->setGeometry(QRect(250, 100, 220, 40));
        label_over->setSizeIncrement(QSize(1, 0));
        label_over->setFont(font2);
        label_over->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_all = new QLabel(credit_wind);
        label_all->setObjectName(QString::fromUtf8("label_all"));
        label_all->setGeometry(QRect(250, 170, 220, 40));
        label_all->setSizeIncrement(QSize(1, 0));
        label_all->setFont(font2);
        label_all->setLineWidth(0);
        label_all->setMidLineWidth(0);
        label_all->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pushButton = new QPushButton(credit_wind);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(250, 231, 220, 40));
        pushButton->setFont(font1);

        retranslateUi(credit_wind);

        QMetaObject::connectSlotsByName(credit_wind);
    } // setupUi

    void retranslateUi(QDialog *credit_wind)
    {
        credit_wind->setWindowTitle(QCoreApplication::translate("credit_wind", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("credit_wind", "\320\241\321\203\320\274\320\274\320\260 \320\272\321\200\320\265\320\264\320\270\321\202\320\260, \321\200\321\203\320\261.", nullptr));
        label_term->setText(QCoreApplication::translate("credit_wind", "\320\241\321\200\320\276\320\272 \320\272\321\200\320\265\320\264\320\270\321\202\320\260, \320\274\320\265\321\201.", nullptr));
        label_3->setText(QCoreApplication::translate("credit_wind", "\320\237\321\200\320\276\321\206\320\265\320\275\321\202\320\275\320\260\321\217 \321\201\321\202\320\260\320\262\320\272\320\260 \320\263\320\276\320\264\320\276\320\262\320\260\321\217, %", nullptr));
        radioButton_ann->setText(QCoreApplication::translate("credit_wind", "\320\220\320\275\320\275\321\203\320\270\321\202\320\265\321\202\320\275\321\213\320\271", nullptr));
        radioButton_dif->setText(QCoreApplication::translate("credit_wind", "\320\224\320\270\321\204\321\204\320\265\321\200\320\265\320\275\321\206\320\270\321\200\320\276\320\262\320\260\320\275\321\213\320\271", nullptr));
        label_4->setText(QCoreApplication::translate("credit_wind", "\320\236\320\261\321\211\320\260\321\217 \320\262\321\213\320\277\320\273\320\260\321\202\320\260, \321\200\321\203\320\261.", nullptr));
        label_5->setText(QCoreApplication::translate("credit_wind", "\320\234\320\260\320\272\321\201. \320\265\320\266\320\265\320\274\320\265\321\201\321\217\321\207\320\275\321\213\320\271 \320\277\320\273\320\260\321\202\320\265\320\266, \321\200\321\203\320\261.", nullptr));
        label_6->setText(QCoreApplication::translate("credit_wind", "\320\237\320\265\321\200\320\265\320\277\320\273\320\260\321\202\320\260, \321\200\321\203\320\261.", nullptr));
        label_month->setText(QString());
        label_over->setText(QString());
        label_all->setText(QString());
        pushButton->setText(QCoreApplication::translate("credit_wind", "\320\222\321\213\320\277\320\276\320\273\320\275\320\270\321\202\321\214 \321\200\320\260\321\201\321\207\320\265\321\202", nullptr));
    } // retranslateUi

};

namespace Ui {
    class credit_wind: public Ui_credit_wind {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREDIT_WIND_H
