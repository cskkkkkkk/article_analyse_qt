#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma once
#include <QDebug>
#include <QMainWindow>
#include<QResizeEvent>
#include<QWidget>
#include<QGridLayout>
#include"AllHeader.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
  void InitMainWindow();

//集成所有的connect函数
    void Connect();

    //槽函数
    void on_pushButtonAuthor_clicked();
    void on_pushButtonSearch_clicked();
    void on_pushButtonCluster_clicked();
    void on_pushButtonHot_clicked();
    void on_pushActionInit();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
