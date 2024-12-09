#ifndef OPTIONS_SEARCH_H
#define OPTIONS_SEARCH_H
#pragma once
#include <QWidget>

namespace Ui {
class options_search;
}

class options_search : public QWidget
{
    Q_OBJECT

public:
    //用于保存当前选中的单选按钮文本
    QString currentRadioButton;
    //用于保存当前输入栏的输入
    QString input;

    explicit options_search(QWidget *parent = nullptr);
    ~options_search();
   void Connect();
   void on_pushButtonReturn_clicked();
   //几个按钮的槽函数,算法函数
   void onRadioButtonAuthor();
   void onRadioButtonTitle();
   void onRadioButtonFuzzy();
   void onRadioButtonCoauthor();

   void onRadioButtonToggled();

   //搜索被点击的槽函数
   void on_pushButtonSearch_clicked();

   std::string QstringToString(QString q);

private:
    Ui::options_search *ui;
};

#endif // OPTIONS_SEARCH_H
