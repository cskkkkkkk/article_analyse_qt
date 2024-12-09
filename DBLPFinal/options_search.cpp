#pragma once
#include "options_search.h"
#include "ui_options_search.h"
#include"AllHeader.h"
options_search::options_search(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::options_search)
{
    ui->setupUi(this);
   this->setLayout(ui->horizontalLayout_Last);
    Connect();
}

options_search::~options_search()
{
    delete ui;
}
std::string options_search::QstringToString(QString q)
{
    std::string str=q.toStdString();
    return str;

}
void options_search::Connect()
{
    connect(ui->pushButtonReturn,&QPushButton::clicked,this,&options_search::on_pushButtonReturn_clicked);
    // 连接信号槽radioButton
    connect(ui->radioButtonAuthor, &QRadioButton::toggled, this, &options_search::onRadioButtonToggled);
    connect(ui->radioButtonTitle,&QRadioButton::toggled, this,&options_search::onRadioButtonToggled);
    connect(ui->radioButtonFuzzy,&QRadioButton::toggled,this,&options_search::onRadioButtonToggled);
    connect(ui->radioButtonCoauthor,&QRadioButton::toggled,this,&options_search::onRadioButtonToggled);


    // 连接textChanged信号到槽函数，将文本保存到userInput中
    connect(ui->lineEdit, &QLineEdit::textChanged, [&](const QString &text){
        this->input = text;
    });

    //连接搜索
    connect(ui->pushButtonSearch,&QPushButton::clicked,this,&options_search::on_pushButtonSearch_clicked);

}

void options_search::on_pushButtonReturn_clicked()
{
    this->close();
    MainWindow*w=new MainWindow;
    w->show();

}

void options_search::onRadioButtonToggled()
{
    QRadioButton *button = qobject_cast<QRadioButton *>(sender());
        if (button && button->isChecked())
        {
            currentRadioButton = button->text();
            qDebug()<< "currentRadioButton is:"<<currentRadioButton;
        }
}

void options_search::onRadioButtonAuthor()
{
    std::string str=QstringToString(input);
    std::string ans;
    std::string root="C:\\Users\\86183\\Desktop\\DblpFolder\\";

    BasicSrarch::main_Reader(str,root,ans);
    std::stringstream sst(ans);
    std::string line;
    //先清除
    ui->listWidget->clear();
    while(std::getline(sst,line,'\n'))
    {
        QString item=QString::fromStdString(line);
        ui->listWidget->addItem(item);


    }

}

void options_search::onRadioButtonTitle()
{

    std::string str=QstringToString(input);
    std::string ans;
    std::string root="C:\\Users\\86183\\Desktop\\DblpFolder\\";

    BasicSrarch::articleInfo(str,root,ans);

    //先清除
   ui->textBrowser->clear();
   QString qans=QString::fromStdString(ans);
   ui->textBrowser->insertPlainText(qans);



}
//Hash4函数
std::string Hash4( std::string& tar) {
    DWORD64 ans = 0;
    for (int i = 0; i < tar.length(); i++) {
        ans = ((ans >> 8) & 0xf) ^ ((ans << 4) ^ tar[i]);
    }
    ans = ans & 0xfff;
    return std::to_string(ans);
}

void options_search::onRadioButtonFuzzy()
{
ui->listWidget->clear();
char url[]="C:\\Users\\86183\\Desktop\\DblpFolder\\";
std::string tar=QstringToString(input);
 qDebug()<<"1";
Fuzzy_Search::initial_fuzzy(url);
 qDebug()<<"2";
//Fuzzy_Search::Full_word_match_fuzzy_search(url,16, tar,fin);
std::string num="";
Fuzzy_Search::Full_word_match_fuzzy_search(url,300, tar,num);
 qDebug()<<"3";
void release_fuzzy_searcher();
 qDebug()<<"4";

 qDebug()<<"5";

 std::string f="C:\\Users\\86183\\Desktop\\DblpFolder\\database\\searchlog\\"+ num+ ".txt";

 QString qss=QString::fromStdString(f);
  qDebug()<<"root is:"<<qss;
 QFile file(qss);
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
         qDebug() << "Could not open file";

     }

     // 创建一个 QTextStream 对象，并将其与 QFile 关联
     QTextStream in(&file);

     // 读取文件的内容并输出到调试控制台
     while (!in.atEnd()) {
         QString line = in.readLine();
         //qDebug() << line;
          ui->listWidget->addItem(line);
     }

     // 关闭文件
     file.close();







//std::string num=tar;
//std::ifstream infile((std::string)url + "database\\searchlog\\" + Hash4(num) + ".txt",std::ios::in);
//if(!infile)
//{
  //  qDebug()<<"文件未成功打开";
//}

//std::string line;
//while(std::getline(infile,line))
//{
  //  QString item=QString::fromStdString(line);
    //ui->listWidget->addItem(item);
//}
 qDebug()<<"6";
//开始添加到listwidget中



}

void options_search::onRadioButtonCoauthor()
{
std::string str=QstringToString(input);
std::string ans;
std::string root="C:\\Users\\86183\\Desktop\\DblpFolder\\";

BasicSrarch::CoAuthor(str,root,ans);
std::stringstream sst(ans);
std::string line;
//先清除
ui->listWidget->clear();
while(std::getline(sst,line,'\n'))
{
    QString item=QString::fromStdString(line);
    ui->listWidget->addItem(item);


}
}

void options_search::on_pushButtonSearch_clicked()
{
if(currentRadioButton=="按作者名查询")
{
    onRadioButtonAuthor();
}
else if(currentRadioButton=="按标题名查询")
{
    onRadioButtonTitle();
}
else if(currentRadioButton=="模糊查询")
{
    onRadioButtonFuzzy();
}
else
{
    onRadioButtonCoauthor();
}

}
