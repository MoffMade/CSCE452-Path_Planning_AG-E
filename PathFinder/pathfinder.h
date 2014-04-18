#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <QObject>
#include <QVector>
#include <QPoint>
#include <QRectF>

struct PointsNode
{
    QPointF point;
    QRectF * rect;

    bool operator<(PointsNode other) const
    {
        return point.x() < other.point.x();
    }
};

/*This class exists purely as an interface, everything is done inside of its one public function.*/

class PathFinder : public QObject
{
    Q_OBJECT

public:
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

    static QVector<QPoint> pathFinder(QRectF window, QRectF r1, QRectF r2, QRectF r3, QPoint start, QPoint goal);

};

#endif // PATHFINDER_H
