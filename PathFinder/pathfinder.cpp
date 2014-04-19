#include <QtAlgorithms>
#include "pathfinder.h"


QVector<QPoint> PathFinder::pathFinder(QRectF window,QRectF r1, QRectF r2, QRectF r3, QPoint start, QPoint goal)
{
    /*1. do a vertical cell decomposition
     *2. construct a graph of all the cells and their interconnections
     *3. use some random weird pathfinding algorithm on the graph
     *4. generate and return QLines for each node, representing the path*/

    /*Step 1
     *
     * 1. sort points by x coordinates
     *      1. create list of pointers to point and parent QRectF
     *      2. sort by points x coordinates
     * 2. draw Qlines from each vertical face of each QRectF.
     *      1. for each QRectF:
     *          1. draw line from topleft to top of field or other QRectF
     *              1. for each QRectF:
     *                  1. if QRectF blocks line from above, add to list
     *              2. find closest QRectF above, if none,  use view field top
     *              3. draw line from topleft() to QRectF.bottomLeft().y()
     *              4. add line to list
     *          2. draw line from topright to top of field or other QRectF
     *              ...
     *          3. draw line from bottomleft() to bottom of field or other QRectF
     *              ...
     *          4. draw line from bottomright() to bottom of field or other QRectF
     * 3. two lines with the same y coordinates define a cell
     *      1. for each line in list:
     *          1. for each line in list except itself:
     *              1. if lines share same y coords, then form a cell
     *              2. if cell collides with obstacles, or other cells, discard, else add to list of cells
     * 4. add Qlines which traverse whole vertical face of each QRectF.
     *      1. for Each QRectF:
     *          1. draw line from top of field or other QRectF to bottom of field or other QRectF with same x
     *          coord as this QRectF along the left side
     *              1. for each QRectF:
     *                  1. if QRectF blocks line from above, add to list for top
     *                  2. if QRectF blocks line from below, add to list for bottom
     *              2. find closest QRectF from top list, else use view field top
     *              3. find closest QRectF from bottom list, else use view field bottom
     *              4. draw QLine from top to bottom with same x coord as this QRectF
     *           2. draw line from top of field or other QRectF to bottom of field or other QRectF with same x
     *          coord as this QRectF along the right side
     * 5. repeat 3
     * 6. DONE :)*/

    QVector<PointsNode> points;
    QVector<QLineF> lines;
    QVector<QRectF*> rects;
    rects.push_back(&r1);
    rects.push_back(&r2);
    rects.push_back(&r3);

    for(int i = 0; i < rects.size(); i++)
    {
        PointsNode node1;
        node1.point = rects[i]->topLeft();
        node1.rect = rects[i];
        points.push_back(node1);
        PointsNode node2;
        node2.point = rects[i]->bottomRight();
        node2.rect = rects[i];
        points.push_back(node2);
    }
    qSort(points.begin(),points.end());

    for (int i = 0; i < rects.size(); i++)
    {
        lines.push_back(QLineF(rects[i]->topLeft(), QPointF(rects[i]->topLeft().x(), findEnd(rects, i, TOPLEFT))));
        lines.push_back(QLineF(rects[i]->topLeft(), QPointF(rects[i]->bottomLeft().x(), findEnd(rects, i,BOTTOMLEFT))));
        lines.push_back(QLineF(rects[i]->topLeft(), QPointF(rects[i]->topRight().x(), findEnd(rects, i, TOPRIGHT))));
        lines.push_back(QLineF(rects[i]->topLeft(), QPointF(rects[i]->bottomRight().x(), findEnd(rects, i, BOTTOMRIGHT))));
    }
}


double PathFinder::findEnd(QVector<QRectF*> rects, int index, LineStart_t startPoint)
{
    QRectF* closestRect = NULL;
    switch(startPoint)
    {

    case TOPLEFT:
        for(int i = 0; i < rects.size(); i ++)
        {
            if(i != index)      //dont check the current rectangle
            {
                if(rects[i]->bottom() > rects[index]->top())        //is it above the current rectangle?
                {
                    if(rects[i]->left() < rects[index]->left() && rects[i]->right() > rects[index]->left())     //is it in the way?
                    {
                        if(closestRect == NULL) closestRect = rects[i];
                        else if(rects[i]->bottom() < closestRect->bottom()) closestRect = rects[i];     //is it the closest?
                    }
                }
            }
        }
        if (closestRect == NULL) return -1;
        else return closestRect->bottom();

    case TOPRIGHT:
        for(int i = 0; i < rects.size(); i ++)
        {
            if(i != index)      //dont check the current rectangle
            {
                if(rects[i]->bottom() > rects[index]->top())        //is it above the current rectangle?
                {
                    if(rects[i]->left() < rects[index]->right() && rects[i]->right() > rects[index]->right())     //is it in the way?
                    {
                        if(closestRect == NULL) closestRect = rects[i];
                        else if(rects[i]->bottom() < closestRect->bottom()) closestRect = rects[i];     //is it the closest?
                    }
                }
            }
        }
        if (closestRect == NULL) return -1;
        else return closestRect->bottom();

    case BOTTOMLEFT:
        for(int i = 0; i < rects.size(); i ++)
        {
            if(i != index)      //dont check the current rectangle
            {
                if(rects[i]->top() < rects[index]->bottom())        //is it below the current rectangle?
                {
                    if(rects[i]->left() < rects[index]->left() && rects[i]->right() > rects[index]->left())     //is it in the way?
                    {
                        if(closestRect == NULL) closestRect = rects[i];
                        else if(rects[i]->top() > closestRect->top()) closestRect = rects[i];     //is it the closest?
                    }
                }
            }
        }
        if (closestRect == NULL) return -1;
        else return closestRect->top();

    case BOTTOMRIGHT:
        for(int i = 0; i < rects.size(); i ++)
        {
            if(i != index)      //dont check the current rectangle
            {
                if(rects[i]->top() < rects[index]->bottom())        //is it below the current rectangle?
                {
                    if(rects[i]->left() < rects[index]->right() && rects[i]->right() > rects[index]->right())     //is it in the way?
                    {
                        if(closestRect == NULL) closestRect = rects[i];
                        else if(rects[i]->top() > closestRect->top()) closestRect = rects[i];     //is it the closest?
                    }
                }
            }
        }
        if (closestRect == NULL) return -1;
        else return closestRect->top();
    }
    return -1;
}
