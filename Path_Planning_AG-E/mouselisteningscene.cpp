#include "mouselisteningscene.h"

mouseListeningScene::mouseListeningScene(QObject *parent) : QGraphicsScene(parent){}
mouseListeningScene::mouseListeningScene(qreal x, qreal y, qreal width, qreal hieght, QObject *parent) :
    QGraphicsScene(x,y,width,hieght,parent){}

void mouseListeningScene::mouseMoveEvent (QGraphicsSceneMouseEvent * mouseEvent){
    //Get the position of the mouse (when clicked)
    QPointF mousePosition=mouseEvent->scenePos();
    //Check if an item is under the mouse
    QGraphicsItem* mouseOver=this->itemAt(mousePosition, *(new QTransform()));
    if(mouseOver!=0){
        //If there is an item, move the center of the item to the mouse position
        mouseOver->grabMouse();
        mouseOver->setPos(mousePosition);
        mouseOver->ungrabMouse();
    }
}
