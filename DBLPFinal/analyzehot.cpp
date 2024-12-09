#pragma once
#include "analyzehot.h"
#include "ui_analyzehot.h"

analyzeHot::analyzeHot(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::analyzeHot)
{
    ui->setupUi(this);
    this->setWindowTitle("年份热点分析");
    setLayout(ui->horizontalLayout_3);

    Connect();

}

analyzeHot::~analyzeHot()
{
    delete ui;
}

void analyzeHot::Connect()
{
    connect(ui->pushButtonReturn,&QPushButton::clicked,this,&analyzeHot::on_pushButtonReturn_clicked);
    connect(ui->pushButtonSearch,&QPushButton::clicked,this,&analyzeHot::on_pushButtonAlgorithom_clicked);

    // 连接textChanged信号到槽函数，将文本保存到userInput中
    connect(ui->lineEdit, &QLineEdit::textChanged, [&](const QString &text){
        this->input = text;
    });
}


void analyzeHot::on_pushButtonReturn_clicked()
{
    this->close();
    MainWindow*w=new MainWindow;
    w->show();

}
void analyzeHot::strTodword()
{
    // 将QString转换为int类型
    bool ok;
    int intValue = input.toInt(&ok);

    if (ok) {
        // 如果转换成功，将int类型转换为DWORD类型
        DWORD dwordValue = static_cast<DWORD>(intValue);
        qDebug() << "DWORD value:" << dwordValue;
    } else {
        qDebug() << "Conversion failed!";
    }


}

//开始搜索
void analyzeHot::on_pushButtonAlgorithom_clicked()
{   //先转换一下
    //strTodword();
    std::string dwordValue=input.toStdString();
    std::string file="C:\\Users\\86183\\Desktop\\DblpFolder\\";
    Keyword_Sorting::Keyword_Sort(dwordValue,file);
    std::ifstream inner((std::string)file + "database\\year_sorted\\" +input.toStdString() + ".txt");
    if(!inner)
    {
        std::cerr<<"未成功打开文件"<<std::endl;
    }
    std::string line;
    int index=0;
    qDebug()<<"1";
    while(std::getline(inner,line))
    {
        std::stringstream sst(line);
        std::string frequency, name;
        sst >> frequency >> name;
       if(name=="for"||name=="of"||name=="and"||name=="in"||name=="A"||name=="the"||name=="on"||name=="with"||name=="to"
               ||name=="With"||name=="a"||name=="An"||name=="The"||name=="an"||name=="by"||name=="Using"||name=="using"||name=="via")
           continue;
        // 转换为QString类型
        QString qFrequency = QString::fromStdString(frequency);
        QString qName = QString::fromStdString(name);

        QTableWidgetItem*item1=new QTableWidgetItem(qName);
        QTableWidgetItem*item2=new QTableWidgetItem(qFrequency);

        ui->tableWidget->setItem(index,0,item1);
        ui->tableWidget->setItem(index,1,item2);
        index++;
    }
     qDebug()<<"2";
}
