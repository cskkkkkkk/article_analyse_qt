#pragma once
#include "myhome.h"
#include "ui_myhome.h"
#include <QLabel>
#include <QMovie>
#include <QVBoxLayout>
MyHome::MyHome(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyHome)
{
    InitMyHome();
    ui->setupUi(this);

}

MyHome::~MyHome()
{
    delete ui;
}

void MyHome::InitMyHome()
{
    // 创建一个 QLabel 并加载 GIF 动画
    QLabel *gifLabel = new QLabel(this);
    QMovie *movie = new QMovie(":/Desktop/DblpFolder/resources/stars-7088_512.gif");
    gifLabel->setMovie(movie);
    movie->start();

    // 将 QLabel 放置在布局中并设置布局属性
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(gifLabel);
    layout->setAlignment(gifLabel, Qt::AlignTop | Qt::AlignLeft);
    layout->setContentsMargins(0, 0, 0, 0);
    gifLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}
