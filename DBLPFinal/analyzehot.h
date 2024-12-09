
#ifndef ANALYZEHOT_H
#define ANALYZEHOT_H
#pragma once
#include <QWidget>
#include<AllHeader.h>
namespace Ui {
class analyzeHot;
}

class analyzeHot : public QWidget
{
    Q_OBJECT

public:
    explicit analyzeHot(QWidget *parent = nullptr);
    ~analyzeHot();
   void Connect();
   void on_pushButtonReturn_clicked();
   QString input;

   void on_pushButtonAlgorithom_clicked();

   void strTodword();
private:
    Ui::analyzeHot *ui;
};

#endif // ANALYZEHOT_H
