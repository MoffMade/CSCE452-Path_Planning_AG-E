#ifndef MOUSELISTENINGSCENE_H
#define MOUSELISTENINGSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QDebug>
class mouseListeningScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit mouseListeningScene(QObject *parent = 0);
    explicit mouseListeningScene(qreal x, qreal y, qreal width, qreal hieght, QObject *parent = 0);

protected:
    void mouseMoveEvent ( QGraphicsSceneMouseEvent * mouseEvent );
};

#endif // MOUSELISTENINGSCENE_H
