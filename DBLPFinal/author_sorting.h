
#ifndef AUTHOR_SORTING_H
#define AUTHOR_SORTING_H
#pragma once
#include <QWidget>
namespace Ui {
class Author_Sorting;
}
class Author_Sorting : public QWidget
{
    Q_OBJECT
public:

    Author_Sorting(QWidget *parent = nullptr);
    ~Author_Sorting();
    //连接槽函数
    void Connect();
    void on_pushButtonReturn_clicked();
    void on_pushButtonAlgorithom_clicked();

private:
    Ui::Author_Sorting *ui;
};

#endif // AUTHOR_SORTING_H
