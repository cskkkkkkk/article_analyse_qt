#pragma once
#include "analyzecluster.h"
#include "ui_analyzecluster.h"
#include"AllHeader.h"
analyzeCluster::analyzeCluster(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::analyzeCluster)
{
    ui->setupUi(this);
    this->setWindowTitle("聚团分析");
    setLayout(ui->verticalLayout_4);

    // 创建图形场景
    scene = new QGraphicsScene(this);
    Connect();


}

analyzeCluster::~analyzeCluster()
{
    delete ui;
}

void analyzeCluster::Connect()
{
    connect(ui->pushButtonReturn,&QPushButton::clicked,this,&analyzeCluster::on_pushButtonReturn_clicked);
    connect(ui->pushButtonBegin,&QPushButton::clicked,this,&analyzeCluster::on_pushButtonBegin_clicked);
    connect(ui->lineEdit, &QLineEdit::textChanged, [&](const QString &text){
        this->input = text;
    });
    connect(ui->pushButtonShow,&QPushButton::clicked,this,&analyzeCluster::on_pushButtonSearch);
}
void analyzeCluster::on_pushButtonSearch()
{
        file="C:\\Users\\86183\\Desktop\\DblpFolder\\database\\aua_result\\"+input+".txt";
        ui->graphicsView->setScene(scene);

        // 读取文件并创建图形
        readNamesFromFile(file);

}

void analyzeCluster::on_pushButtonReturn_clicked()
{
    this->close();
    MainWindow*w=new MainWindow;
    w->show();

}

void analyzeCluster::on_pushButtonBegin_clicked()
{

    // 创建 QFile 对象并打开文件
    QFile file("C:\\Users\\86183\\Desktop\\DblpFolder\\database\\aua_rank_result.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: Cannot open file!";
        return;
    }

    // 创建 QTextStream 对象并与 QFile 关联
    QTextStream in(&file);

    // 逐行读取文件内容并输出
    qDebug() << "File content:";
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(" "); // 使用空格分割成两部分

        if (parts.size() >= 2) {
            QString firstPart = parts[0]; // 第一个部分
            QString secondPart = parts[1]; // 第二个部分

        ui->listWidget->addItem("存储的文件名称为："+firstPart+".txt"+"      "+"阶数为:"+secondPart);
        }
    }

    // 关闭文件
    file.close();
}

void analyzeCluster::readNamesFromFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QStringList nodeNames;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        nodeNames.append(line);
    }

    file.close();

    createGraph(nodeNames);
}





/*
void analyzeCluster::createGraph(const QStringList &nodeNames) {
    int margin = 800;     // 边缘间距
    int radius = 2;      // 节点半径

    // 设置字体样式
    QFont textFont("Arial", 3);

    // 在图形场景中添加随机分布的节点
    QList<QGraphicsEllipseItem *> nodes;

    // 使用 QRandomGenerator 生成随机坐标
    QRandomGenerator randomGenerator(static_cast<quint32>(QTime::currentTime().msec()));

    for (int i = 0; i < nodeNames.size(); i++) {
        QString nodeName = nodeNames.at(i);

        // 在列表控件中添加节点名字
        QListWidgetItem *listItem = new QListWidgetItem(nodeName);

        // 生成随机坐标以布局节点
        qreal randomX = randomGenerator.bounded(scene->width() - 4 * radius - 2 * margin) + margin + 2 * radius;
        qreal randomY = randomGenerator.bounded(scene->height() - 4 * radius - 2 * margin) + margin + 2 * radius;

        // 在图形场景中添加圆形节点
        QGraphicsEllipseItem *nodeEllipse = scene->addEllipse(randomX - radius, randomY - radius, radius*2, radius*2);
        nodeEllipse->setBrush(Qt::lightGray);

        // 添加作者名字标签
        QGraphicsTextItem *textItem = scene->addText(nodeName, textFont);
        textItem->setPos(randomX - radius, randomY + radius + 10); // 放置名字标签的位置

        // 为圆形节点添加到节点列表
        nodes.append(nodeEllipse);
    }

    // 为圆形节点添加边，连接任意两个节点
    for (int i = 0; i < nodes.size()-1; i++) {
        for (int j = i + 1; j < nodes.size(); j++) {
            QGraphicsEllipseItem *node1 = nodes.at(i);
            QGraphicsEllipseItem *node2 = nodes.at(j);

            if (node1 && node2) {
                QPointF center1 = node1->rect().center();
                QPointF center2 = node2->rect().center();

                // 添加连线连接任意两个节点
                QGraphicsLineItem *line = scene->addLine(center1.x(), center1.y(), center2.x(), center2.y());
                line->setPen(QPen(Qt::black, 2));
            }
        }
    }



    // 缩放视图以适应所有节点
    ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);

    // 启用鼠标缩放功能
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    ui->graphicsView->setMouseTracking(true);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsView->viewport()->installEventFilter(this);
}
*/
void analyzeCluster::createGraph(const QStringList &nodeNames) {
    int margin = 1000;     // 边缘间距
    int radius = 5;        // 节点半径
    int spacing = 100;     // 节点间距

    // 设置字体样式
    QFont textFont("Arial", 8); // 调整文本大小

    // 在图形场景中添加节点
    QList<QGraphicsEllipseItem *> nodes;

    // 计算一行可以容纳的节点数量
    int nodesPerRow = 6;
    int rowCount = qCeil(static_cast<float>(nodeNames.size()) / nodesPerRow);

    // 计算节点之间的实际间距
    qreal totalWidth = scene->width() - 2 * margin;
    qreal totalHeight = scene->height() - 2 * margin;
    qreal actualSpacingX = (totalWidth - 2 * radius * nodesPerRow) / (nodesPerRow - 1);
    qreal actualSpacingY = (totalHeight - 2 * radius * rowCount) / (rowCount - 1);

    // 添加节点
    for (int i = 0; i < nodeNames.size(); i++) {
        QString nodeName = nodeNames.at(i);

        // 计算节点的行列号
        int row = i / nodesPerRow;
        int col = i % nodesPerRow;

        // 计算节点位置
        qreal x = margin + col * (2 * radius + actualSpacingX);
        qreal y = margin + row * (2 * radius + actualSpacingY);

        // 添加圆形节点
        QGraphicsEllipseItem *nodeEllipse = scene->addEllipse(x - radius, y - radius, 2 * radius, 2 * radius);
        nodeEllipse->setBrush(Qt::lightGray);

        // 添加作者名字标签
        QGraphicsTextItem *textItem = scene->addText(nodeName, textFont);
        textItem->setPos(x - radius, y + 2 * radius + 10); // 调整文本位置，在圆形节点下方
        textItem->setRotation(0); // 设置文本不旋转

        // 添加到节点列表
        nodes.append(nodeEllipse);
    }

    // 计算放大比例
    qreal scaleX = (scene->width() - margin * 2) / (totalWidth + actualSpacingX * (nodesPerRow - 1));
    qreal scaleY = (scene->height() - margin * 2) / (totalHeight + actualSpacingY * (rowCount - 1));
    qreal scale = qMin(scaleX, scaleY);

    // 缩放视图以适应所有节点并保持节点间距
    ui->graphicsView->scale(scale, scale);

    // 逆时针旋转180度
    ui->graphicsView->rotate(180);

    // 设置视图中心为场景中心
    ui->graphicsView->centerOn(scene->width() / 2, scene->height() / 2);

    // 启用鼠标缩放功能
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    ui->graphicsView->setMouseTracking(true);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsView->viewport()->installEventFilter(this);
}


void analyzeCluster::wheelEvent(QWheelEvent *event) {
    // 鼠标滚轮事件处理函数
    qreal scaleFactor = 1.2; // 缩放因子

    if (event->delta() > 0) {
        // 滚轮向上，放大视图
        ui->graphicsView->scale(scaleFactor, scaleFactor);
    } else {
        // 滚轮向下，缩小视图
        ui->graphicsView->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
}
