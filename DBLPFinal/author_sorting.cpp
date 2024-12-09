#pragma once
#include "author_sorting.h"
#include"ui_author_sorting.h"
#include"QWidget"
#include"AllHeader.h"
Author_Sorting::Author_Sorting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Author_Sorting)
{
    ui->setupUi(this);
    this->setWindowTitle("年份热点分析");
    setLayout(ui->gridLayout);

    Connect();
}

Author_Sorting::~Author_Sorting()
{
    delete ui;
}
void Author_Sorting::Connect()
{
    connect(ui->pushButtonReturn,&QPushButton::clicked,this,&Author_Sorting::on_pushButtonReturn_clicked);
    connect(ui->pushButtonBegin,&QPushButton::clicked,this,&Author_Sorting::on_pushButtonAlgorithom_clicked);

}
void Author_Sorting::on_pushButtonReturn_clicked()
{
    this->close();
    MainWindow*w=new MainWindow;
    w->show();

}
void Author_Sorting::on_pushButtonAlgorithom_clicked()
{   char file[]="C:\\Users\\86183\\Desktop\\DblpFolder\\database\\author_rank_result";
    std::ifstream infile(file);
    if(!infile)
    {
          std::cerr<<"can't open file"<<std::endl;
    }
    std::string line;
    int index=0;
    while(std::getline(infile,line))
    {
        std::stringstream sst(line);
        std::string name,num;
        if(sst>>num)
        {
             std::getline(sst >> std::ws, name);
        }

        // 转换为QString类型
        QString qFrequency = QString::fromStdString(num);
        QString qName = QString::fromStdString(name);

        QTableWidgetItem*item1=new QTableWidgetItem(qName);
        QTableWidgetItem*item2=new QTableWidgetItem(qFrequency);
        ui->tableWidget->setItem(index,0,item1);
        ui->tableWidget->setItem(index,1,item2);
        index++;

    }

}
