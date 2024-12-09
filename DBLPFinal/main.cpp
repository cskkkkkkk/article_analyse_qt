#pragma once
#include "mainwindow.h"
#include"author_sorting.h"
#include <QApplication>
#include"options_search.h"
#include"analyzehot.h"
#include"analyzecluster.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
   // Author_Sorting as;
    //as.show();
   //analyzeCluster opser;
    //opser.show();
    return a.exec();
}
