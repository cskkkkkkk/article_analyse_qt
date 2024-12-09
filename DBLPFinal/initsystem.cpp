#include "initsystem.h"
#include "ui_initsystem.h"

InitSystem::InitSystem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InitSystem)
{
    ui->setupUi(this);
    this->setWindowTitle("数据处理初始化");
    setLayout(ui->gridLayout);
Connect();
}

InitSystem::~InitSystem()
{
    delete ui;
}
void InitSystem::Connect()
{
    connect(ui->pushButton_2,&QPushButton::clicked,this,&InitSystem::on_pushButtonReturn);
    connect(ui->pushButton,&QPushButton::clicked,this,&InitSystem::on_pushButtonInit);
    connect(ui->lineEdit, &QLineEdit::textChanged, [&](const QString &text){
        this->input = text;
    });
}
void InitSystem::on_pushButtonReturn()
{
    this->close();
    MainWindow *mw=new MainWindow;
    mw->show();
}

void InitSystem::on_pushButtonInit()
{
    std::string file=input.toStdString();
    data::initial_readers(300,300,false,file);
}
