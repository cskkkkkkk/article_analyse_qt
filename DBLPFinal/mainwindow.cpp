#pragma once
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QPainter>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//初始化窗口格式
    InitMainWindow();


         //开启所有connect
             Connect();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitMainWindow()
{

    this->setWindowTitle("DBLP文献管理");
    // 创建一个QWidget作为GridLayout布局的容器
    QWidget *gridLayoutContainer = new QWidget(this);
    setCentralWidget(gridLayoutContainer);

    // 设置GridLayout布局
    gridLayoutContainer->setLayout(ui->gridLayout);

    // 加载QSS样式文件
       QFile styleFile("C:\\Users\\86183\\Desktop\\DblpFolder\\style");
       styleFile.open(QFile::ReadOnly);
       QString style = QLatin1String(styleFile.readAll());
       qApp->setStyleSheet(style);




    // 加载背景图片
        QPixmap bgPixmap(":/Desktop/DblpFolder/resources/background.jpg");

        // 创建绘图对象，并在其上绘制文本
        QPainter painter(&bgPixmap);
        painter.setPen(Qt::black); // 设置文本颜色为黑色

        // 设置字体
        QFont font("Arial", 30);
        painter.setFont(font);

        // 获取文本大小
        QFontMetrics fm(font);
        QSize textSize = fm.size(Qt::TextSingleLine, "DBLP文献管理系统");

        // 计算文本的位置（居中显示）
        int x = (bgPixmap.width() - textSize.width()) / 2;
        int y = (bgPixmap.height() - textSize.height()) / 2;

        // 在背景图片上绘制文本
        painter.drawText(x, y, "DBLP文献管理系统");


        // 设置背景图片到QLabel
           ui->mainLabel->setPixmap(bgPixmap);
           ui->mainLabel->setScaledContents(true); // 图片自适应大小
           //ui->gridLayout->addStretch();
           // 设置QLabel在垂直布局中的大小
         //  ui->verticalLayout->setStretchFactor(ui->detailLabel, 0.1); // 设置QLabel的伸展因子为1，使其占据整个垂直空间
           // 设置detailLabel的大小策略为Preferred
            // ui->detailLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

}


void MainWindow::Connect()
{
     // 连接Exit选项的触发信号到槽函数，关闭窗口
     connect(ui->actionexit,&QAction::triggered, this, &MainWindow::close);
     //connect(,&QAction::triggered, this,&MainWindow::on_pushActionInit);
    // connect(,&QAction::triggered, this,&MainWindow::on_pushActionInit);
    connect(ui->actionInit,&QAction::triggered, this, &MainWindow::on_pushActionInit);
     //基本按钮连接
     connect(ui->pushButtonSearch,&QPushButton::clicked, this,&MainWindow::on_pushButtonSearch_clicked);
     connect(ui->pushButtonHot,&QPushButton::clicked, this,&MainWindow::on_pushButtonHot_clicked);
     connect(ui->pushButtonAuthor,&QPushButton::clicked, this,&MainWindow::on_pushButtonAuthor_clicked);
     connect(ui->pushButtonCluster,&QPushButton::clicked,this,&MainWindow::on_pushButtonCluster_clicked);
}
void MainWindow::on_pushActionInit()
{
    this->close();
     InitSystem*init=new  InitSystem;
     init->show();
}

void MainWindow::on_pushButtonAuthor_clicked()
{//关闭当前窗口
    this->close();
    //打开其它窗口
    Author_Sorting *as=new Author_Sorting;
    as->show();

}
void MainWindow::on_pushButtonSearch_clicked()
{
    this->close();
    options_search *sear=new options_search;
    sear->show();

}

void MainWindow::on_pushButtonHot_clicked()
{
    this->close();
    analyzeHot *hot=new analyzeHot;
    hot->show();

}

void MainWindow::on_pushButtonCluster_clicked()
{
   this->close();
    analyzeCluster* cluster=new analyzeCluster;
    cluster->show();
}
