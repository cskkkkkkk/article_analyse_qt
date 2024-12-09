#ifndef ANALYZECLUSTER_H
#define ANALYZECLUSTER_H
#pragma once
#include <QWidget>
#include"AllHeader.h"
namespace Ui {
class analyzeCluster;
}

class analyzeCluster : public QWidget
{
    Q_OBJECT

public:
    explicit analyzeCluster(QWidget *parent = nullptr);
    ~analyzeCluster();
    void Connect();
    void on_pushButtonReturn_clicked();
    QString input;
    void on_pushButtonBegin_clicked();
    void createGraph(const QStringList &nodeNames);
    void readNamesFromFile(const QString &filePath);
    QString file;
    //开始展示
    void on_pushButtonSearch();
    void wheelEvent(QWheelEvent *event);
 private:
    Ui::analyzeCluster *ui;
      QGraphicsScene *scene;
};

#endif // ANALYZECLUSTER_H
