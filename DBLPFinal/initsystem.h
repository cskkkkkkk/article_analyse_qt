#ifndef INITSYSTEM_H
#define INITSYSTEM_H
#pragma once
#include <QWidget>
#include"AllHeader.h"
namespace Ui {
class InitSystem;
}

class InitSystem : public QWidget
{
    Q_OBJECT

public:
    explicit InitSystem(QWidget *parent = nullptr);
    ~InitSystem();
    void Connect();
    void on_pushButtonReturn();
    void on_pushButtonInit();
    QString input;
private:
    Ui::InitSystem *ui;
};

#endif // INITSYSTEM_H
