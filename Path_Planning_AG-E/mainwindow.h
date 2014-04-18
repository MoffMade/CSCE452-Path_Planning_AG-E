#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//Used for block sizing
#define BLK1 200
#define BLK2 150
#define BLK3 100
//Used for drawing start/goal markers (MarKer Radius)
#define MKR 20

#include <QMainWindow>
#include <QEvent>
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QPoint>
#include <QVector>
#include <QDebug>
#include "mouselisteningscene.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_paintButton_clicked();
    void on_quitButton_clicked();
    void on_resetButton_clicked();

private:
    Ui::MainWindow *ui;
    mouseListeningScene *simScene;
    QVector<QGraphicsLineItem*> pathLines;
    QGraphicsRectItem *block1, *block2, *block3;
    QGraphicsEllipseItem *startMarker, *goalMarker;
    void paintPath(QVector<QPointF> vertices);
    void setUpItems();
    void setUpSlots();

};

#endif // MAINWINDOW_H
