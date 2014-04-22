#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <QObject>
#include <QVector>
#include <QPoint>
#include <QRectF>
#include <QLine>

/*This class exists purely as an interface, everything is done inside of its one public function.*/

struct Node
{
public:
    QRectF* cell;
    QVector<Node*> connections;
    int number;       //only for debugging
};

class PathFinder : public QObject
{
    Q_OBJECT

public:
    static enum LineStart_t{TOPLEFT, TOPRIGHT, BOTTOMLEFT, BOTTOMRIGHT} LineStart;

    /************************************************************************************************************
     * finds the path through the obstacle field
     *
     *Inputs:
     *  QRectF window - the rectangle which bounds the robot's world; can be the QGraphicsView's viewing rectange
     *  QRectF r1, r2, r3 - the three rectangles which represent the obstacles in the world
     *  QPoint start - the starting point of the robot
     *  QPoint goal - the position the robot should navigate to
     *
     *Returns:
     *  returns a QVector<QPoint> which is a list of points which the robot should travel to in order.
     *  these points represent the endpoints of QLines which should be drawn on the display. 3 points means 2
     *  lines, one from point1 to point 2, and another from point 2 to point 3.
     ***********************************************************************************************************/

    static QVector<QPointF> pathFinder(QRectF window, QRectF r1, QRectF r2, QRectF r3, QPointF start, QPointF goal);
    static QVector<QRectF> pathFinderRect(QRectF window, QRectF r1, QRectF r2, QRectF r3, QPointF start, QPointF goal);

private:
    static QVector<QRectF> cellDecomposition(QRectF window, QRectF r1, QRectF r2, QRectF r3);
    static Node* generateGraph(QVector<QRectF> cells);
    static QVector<QPointF> findPath(Node* head, QPointF startPoint, QPointF goalPoint);
    static Node* findPointInCell(Node* root, QPointF target, QVector<Node*> *visited);
    static Node* cellPath(Node* root, Node *target, QVector<Node*> *cells);
    static double findEnd(QVector<QRectF*> rects, QRectF *window, int index, LineStart_t startPoint);
    static bool noCollide(QVector<QRectF*> rects, QVector<QRectF> cells, QRectF cell);
    static void findCells(QVector<QRectF> *cells, QVector<QLineF> lines, QVector<QRectF*> rects);
    static void removeExtraneous(QVector<QRectF> *cells);
};

#endif // PATHFINDER_H
