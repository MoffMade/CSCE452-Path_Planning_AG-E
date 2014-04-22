#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pathfinder.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    simScene=new mouseListeningScene(0,0,500,500,this);
    ui->simView->setScene(simScene);
    setUpItems();
    setUpSlots();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintPath(QVector<QPointF> vertices){
    //Ensure at least two points are in vertices
    if(vertices.size()>=2){
        QPen linePen(Qt::red, 4, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin);
        for(int i=1;i<vertices.size();i++){
            //Create a QGraphicsLineItem connecting each sequential vertex of the path
            QGraphicsLineItem* tempLine=new QGraphicsLineItem();
            tempLine->setPen(linePen);
            tempLine->setLine(vertices[i-1].x(),vertices[i-1].y(),vertices[i].x(),vertices[i].y());
            //Add the line item to the global vector
            pathLines.push_back(tempLine);
            simScene->addItem(tempLine);
        }
    }
    else{
        qDebug()<<"paintPath function: QVector vertices size <2, no path created.";
    }
}

void MainWindow::on_paintButton_clicked()
{
    //Remove any previous path if present
    if(pathLines.size()!=0){
        for(int i=0; i<pathLines.size(); i++){
            simScene->removeItem(pathLines[i]);
        }
        pathLines.clear();
    }
    //Remove any previous cells if present
    if(cells.size()!=0){
        for(int i=0; i<cells.size(); i++){
            simScene->removeItem(cells[i]);
        }
        cells.clear();
    }

    QRectF r1, r2, r3;
    r1 = block1->mapRectToScene(block1->boundingRect());
    r2 = block2->mapRectToScene(block2->boundingRect());
    r3 = block3->mapRectToScene(block3->boundingRect());

    //Draw Cells.
    QVector<QRectF> graphCells = PathFinder::pathFinderRect(simScene->sceneRect(), r1, r2, r3, startMarker->pos(), goalMarker->pos());
    for (int i = 0; i < graphCells.size(); i++)
    {
        QGraphicsRectItem *cell=new QGraphicsRectItem(graphCells[i]);
        cell->setPen(QPen(Qt::blue, 2));
        simScene->addItem(cell);
        cells.push_back(cell);
    }

    //Draw New Path
    QVector<QPointF> vertices = PathFinder::pathFinder(simScene->sceneRect(), r1, r2, r3, startMarker->pos(), goalMarker->pos());
    paintPath(vertices);
}

void MainWindow::on_quitButton_clicked()
{
    //Close application
    qApp->quit();
}

void MainWindow::on_resetButton_clicked()
{
    //Clear path for new path drawing
    for(int i=0; i<pathLines.size(); i++){
        simScene->removeItem(pathLines[i]);
    }    
    pathLines.clear();
    for(int i=0; i<cells.size(); i++){
        simScene->removeItem(cells[i]);
    }
    cells.clear();

}

void MainWindow::setUpItems(){
    //Used to initiate members
    //x,y,w,and l decided to that the item's local coord system starts in center of graphic
    block1=new QGraphicsRectItem(-BLK1/2,-BLK1/2,BLK1,BLK1);
    block2=new QGraphicsRectItem(-BLK2/2,-BLK2/2,BLK2,BLK2);
    block3=new QGraphicsRectItem(-BLK3/2,-BLK3/2,BLK3,BLK3);
    startMarker=new QGraphicsEllipseItem(-MKR/2,-MKR/2,MKR,MKR);
    goalMarker=new QGraphicsEllipseItem(-MKR/2,-MKR/2,MKR,MKR);

    //set default positions of items in scene coord system
    block1->setPos(150,200);
    block2->setPos(350,150);
    block3->setPos(150,400);
    startMarker->setPos(20,20);
    goalMarker->setPos(480,480);

    //Brushes for painting blocks and start/goal
    QBrush *blockBrush=new QBrush(Qt::darkGray, Qt::SolidPattern);
    QBrush *startBrush=new QBrush(Qt::darkRed, Qt::SolidPattern);
    QBrush *goalBrush=new QBrush(Qt::green, Qt::SolidPattern);
    block1->setBrush(*blockBrush);
    block2->setBrush(*blockBrush);
    block3->setBrush(*blockBrush);
    startMarker->setBrush(*startBrush);
    goalMarker->setBrush(*goalBrush);

    //Add each block, start, and goal to scene
    simScene->addItem(block1);
    simScene->addItem(block2);
    simScene->addItem(block3);
    simScene->addItem(startMarker);
    simScene->addItem(goalMarker);
}

void MainWindow::setUpSlots(){
    //connect button signals to proper slot functions
    connect(ui->paintButton, SIGNAL(clicked()), this,SLOT(on_paintButton_clicked()));
    connect(ui->resetButton, SIGNAL(clicked()), this,SLOT(on_resetButton_clicked()));
    connect(ui->quitButton, SIGNAL(clicked()), this,SLOT(on_quitButton_clicked()));
}
