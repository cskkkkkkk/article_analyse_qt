#ifndef MYHOME_H
#define MYHOME_H
#pragma once
#include <QWidget>

namespace Ui {
class MyHome;
}

class MyHome : public QWidget
{
    Q_OBJECT

public:
    explicit MyHome(QWidget *parent = nullptr);
    ~MyHome();
   void InitMyHome();
private:
    Ui::MyHome *ui;
};

#endif // MYHOME_H
