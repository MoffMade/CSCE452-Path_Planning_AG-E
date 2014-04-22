#include <QtAlgorithms>
#include "pathfinder.h"
#include <QDebug>


QVector<QPointF> PathFinder::pathFinder(QRectF window, QRectF r1, QRectF r2, QRectF r3, QPointF start, QPointF goal)
{
    /*1. do a vertical cell decomposition
     *2. construct a graph of all the cells and their interconnections
     *3. use some random weird pathfinding algorithm on the graph
     *4. generate and return QLines for each node, representing the path*/

    /*Step 1
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

    QVector<QRectF> cells = cellDecomposition(window, r1, r2, r3);
    Node* headNode;
    headNode = generateGraph(cells);
    return findPath(headNode, start, goal);

}
QVector<QRectF> PathFinder::pathFinderRect(QRectF window, QRectF r1, QRectF r2, QRectF r3, QPointF start, QPointF goal)
{
    QVector<QRectF> cells = cellDecomposition(window, r1, r2, r3);
    Node* headNode;
    headNode = generateGraph(cells);
    return cells;

}
QVector<QPointF> PathFinder::findPath(Node* head, QPointF startPoint, QPointF goalPoint)
{
    //findPointInCell - Find which cell start and goal are in
    //cellPath - Find a list of connected cells from startCell to goalCell
    qDebug()<<"Starting Path Finding...";
    Node *startCell, *goalCell;
    QVector<Node*> *cells=new QVector<Node*>;
    QVector<Node*> *visitedStart=new QVector<Node*>;
    QVector<Node*> *visitedGoal=new QVector<Node*>;
    QVector<QPointF> pathPoints;
    startCell=findPointInCell(head,startPoint,visitedStart);
    goalCell=findPointInCell(head,goalPoint,visitedGoal);
    qDebug()<<"Found cell containing start and goal points.";
    Node *path=cellPath(startCell,goalCell,cells);
    if(path!=NULL){
        qDebug()<<"Found path between points. Cell path is:";
        for(int i=0; i<cells->size();i++)
            qDebug()<<cells->at(i)->number;
        qDebug()<<"Building line paths...";
        //Build QPoint Path
        //First Point (Start)
        pathPoints.push_back(startPoint);
        //Build based on cells vector
        //  -Adds points on edge midpoints for intermediate cells
        //   (up until reaching the final cell)
        for(int i=1;i<cells->size();i++){
            //Find closet neighboring edge of next cell.
            qDebug()<<"Building path line"<<i;
            QPointF nextPoint;
            QRectF currentCell=*(cells->at(i-1)->cell);
            QRectF nextCell=*(cells->at(i)->cell);
            if( currentCell.x() < nextCell.x() ){
                //Path moving RIGHT
                if( currentCell.height() < nextCell.height() ){
                    //current cell has smaller edge
                    nextPoint.setX( currentCell.topRight().x() );
                    nextPoint.setY( currentCell.topRight().y() + (currentCell.height()/2) );
                    pathPoints.push_back(nextPoint);
                }
                else if( currentCell.height() > nextCell.height() ){
                    //next cell has smaller edge
                    nextPoint.setX( nextCell.topLeft().x() );
                    nextPoint.setY( nextCell.topLeft().y() + (nextCell.height()/2) );
                    pathPoints.push_back(nextPoint);
                }
            }
            else if( currentCell.x() > nextCell.x() ){
                //Path moving LEFT
                if( currentCell.height() < nextCell.height() ){
                    //current cell has smaller edge
                    nextPoint.setX( currentCell.topLeft().x() );
                    nextPoint.setY( currentCell.topLeft().y() + (currentCell.height()/2) );
                    pathPoints.push_back(nextPoint);
                }
                else if( currentCell.height() > nextCell.height() ){
                    //next cell has smaller edge
                    nextPoint.setX( nextCell.topRight().x() );
                    nextPoint.setY( nextCell.topRight().y() + (nextCell.height()/2) );
                    pathPoints.push_back(nextPoint);
                }
            }
        }
        //All intermediate points added, add goal point to finish.
        qDebug()<<"Building final path line";
        pathPoints.push_back(goalPoint);
        return pathPoints;
    }
    else{
        //No Path Found
        pathPoints.push_back(startPoint);
        qDebug()<<"No Path Found.";
        return pathPoints;
    }

}

Node* PathFinder::findPointInCell(Node* root, QPointF target, QVector<Node *> *visited){
    //root node contains point
    qDebug()<<"Searching graph for cell containing ("<<target.x()<<","<<target.y()<<")...";
    visited->push_back(root);
    if(root->cell->contains(target.x(),target.y())){
        return root;
    }
    //root node has no further connections
    else if(root->connections.size()<=1){
        return NULL;
    }
    //root has further connections, check each.
    else{
        for(int i=0; i<root->connections.size(); i++){
            //traverse rest of graph using each root until target is found
            //ensure no repitions
            if(!(visited->contains(root->connections[i]))){
                Node* next = findPointInCell(root->connections[i],target, visited);
                if(next!=NULL){
                    //Check to ensure point is contained
                    if(next->cell->contains(target.x(),target.y())){
                        //return cell containing point
                        return next;
                    }
                }
            }
            //Point not in this connections branch, try next
        }
        //None of root's connection's branchs contain point.
        return NULL;
    }
}

Node* PathFinder::cellPath(Node* root, Node *target, QVector<Node*> *cells){
    //Check node and connection branchs for connection to target.
    //Add each node to cells vector to track path.
    qDebug()<<"Searching for path...";
    //Add cell to path
    cells->push_back(root);
    if(root==target){
        //The current node is the target
        return root;
    }
    else if(root->connections.contains(target)){
        //root has target in connections vector.
        return cellPath(root->connections.at(root->connections.indexOf(target)),target,cells);
    }
    else{
        //Check new cell
        for(int i=0; i<root->connections.size(); i++){
            //check if connections[i] has already been visited (present in cells vector)
            if(!(cells->contains(root->connections[i])))
                //if it has not been visited, visit.
                return cellPath(root->connections[i],target,cells);
        }
        //No path exists from root to target
        return NULL;
    }
}

Node* PathFinder::generateGraph(QVector<QRectF> cells)
{
    /*if two cells share an edge of some kind, they are connected
     *the y coordinates of the two edges must overlap*/

    //create nodes from cells
    QVector <Node*> nodes;
    for(int i = 0; i < cells.size(); i++)
    {
        Node* node = new Node();
        node->cell = &cells[i];
        node->number = i;
        nodes.push_back(node);
    }

    for (int i = 0; i < nodes.size(); i++)
    {
        for (int j = 0; j < nodes.size(); j++)
        {
            if (j != i){
                if(nodes[i]->cell->left() == nodes[j]->cell->right())
                {
                    if((nodes[i]->cell->top() <= nodes[j]->cell->top() && nodes[i]->cell->bottom() >= nodes[j]->cell->top()) ||
                            (nodes[i]->cell->bottom() >= nodes[j]->cell->bottom() && nodes[i]->cell->top() <= nodes[j]->cell->bottom()))
                    {
                        nodes[i]->connections.push_back(nodes[j]);
                        nodes[j]->connections.push_back(nodes[i]);
                    }
                }
            }
        }
    }
    qDebug() <<"nodes\n";
    for(int i = 0; i < nodes.size(); i++)
    {
        qDebug() <<"node: " << i <<" "<< nodes[i]->number;
        for(int j = 0; j < nodes[i]->connections.size(); j++)
        {
            qDebug() << nodes[i]->connections[j]->number;
        }
    }
    return nodes[0];
}

QVector<QRectF> PathFinder::cellDecomposition(QRectF window, QRectF r1, QRectF r2, QRectF r3)
{
    QVector<QLineF> lines;
    QVector<QRectF*> rects;
    rects.push_back(&r1);
    rects.push_back(&r2);
    rects.push_back(&r3);

    for (int i = 0; i < rects.size(); i++)
    {
        lines.push_back(QLineF(rects[i]->topLeft(), QPointF(rects[i]->topLeft().x(), findEnd(rects, &window, i, TOPLEFT))));
        lines.push_back(QLineF(rects[i]->bottomLeft(), QPointF(rects[i]->bottomLeft().x(), findEnd(rects, &window, i,BOTTOMLEFT))));
        lines.push_back(QLineF(rects[i]->topRight(), QPointF(rects[i]->topRight().x(), findEnd(rects, &window, i,  TOPRIGHT))));
        lines.push_back(QLineF(rects[i]->bottomRight(), QPointF(rects[i]->bottomRight().x(), findEnd(rects, &window, i, BOTTOMRIGHT))));
    }

    QVector<QRectF> cells;
    findCells(&cells, lines, rects);

    for (int i = 0; i < rects.size(); i++)
    {
        lines.push_back(QLineF(QPointF(rects[i]->topLeft().x(), findEnd(rects, &window, i, TOPLEFT)), QPointF(rects[i]->topLeft().x(), findEnd(rects, &window, i, BOTTOMLEFT))));
        lines.push_back(QLineF(QPointF(rects[i]->topRight().x(), findEnd(rects, &window, i, TOPRIGHT)), QPointF(rects[i]->topRight().x(), findEnd(rects, &window, i, BOTTOMRIGHT))));
        lines.push_back(QLineF(window.topLeft(),window.bottomLeft()));
        lines.push_back(QLineF(window.topRight(), window.bottomRight()));
    }

    findCells(&cells, lines, rects);

    removeExtraneous(&cells);

    return cells;
}

double PathFinder::findEnd(QVector<QRectF*> rects, QRectF* window, int index, LineStart_t startPoint)
{
    QRectF* closestRect = NULL;
    switch(startPoint)
    {

    case TOPLEFT:
        for(int i = 0; i < rects.size(); i ++)
        {
            if(i != index)      //dont check the current rectangle
            {
                if(rects[i]->bottom() < rects[index]->top())        //is it above the current rectangle?
                {
                    if(rects[i]->left() < rects[index]->left() && rects[i]->right() > rects[index]->left())     //is it in the way?
                    {
                        if(closestRect == NULL) closestRect = rects[i];
                        else if(rects[i]->bottom() < closestRect->bottom()) closestRect = rects[i];     //is it the closest?
                    }
                }
            }
        }
        if (closestRect == NULL) return window->top();
        else return closestRect->bottom();

    case TOPRIGHT:
        for(int i = 0; i < rects.size(); i ++)
        {
            if(i != index)      //dont check the current rectangle
            {
                if(rects[i]->bottom() < rects[index]->top())        //is it above the current rectangle?
                {
                    if(rects[i]->left() < rects[index]->right() && rects[i]->right() > rects[index]->right())     //is it in the way?
                    {
                        if(closestRect == NULL) closestRect = rects[i];
                        else if(rects[i]->bottom() < closestRect->bottom()) closestRect = rects[i];     //is it the closest?
                    }
                }
            }
        }
        if (closestRect == NULL) return window->top();
        else return closestRect->bottom();

    case BOTTOMLEFT:
        for(int i = 0; i < rects.size(); i ++)
        {
            if(i != index)      //dont check the current rectangle
            {
                if(rects[i]->top() > rects[index]->bottom())        //is it below the current rectangle?
                {
                    if(rects[i]->left() < rects[index]->left() && rects[i]->right() > rects[index]->left())     //is it in the way?
                    {
                        if(closestRect == NULL) closestRect = rects[i];
                        else if(rects[i]->top() > closestRect->top()) closestRect = rects[i];     //is it the closest?
                    }
                }
            }
        }
        if (closestRect == NULL) return window->bottom();
        else return closestRect->top();

    case BOTTOMRIGHT:
        for(int i = 0; i < rects.size(); i ++)
        {
            if(i != index)      //dont check the current rectangle
            {
                if(rects[i]->top() > rects[index]->bottom())        //is it below the current rectangle?
                {
                    if(rects[i]->left() < rects[index]->right() && rects[i]->right() > rects[index]->right())     //is it in the way?
                    {
                        if(closestRect == NULL) closestRect = rects[i];
                        else if(rects[i]->top() > closestRect->top()) closestRect = rects[i];     //is it the closest?
                    }
                }
            }
        }
        if (closestRect == NULL) return window->bottom();
        else return closestRect->top();
    }
}

void PathFinder::findCells(QVector<QRectF>* cells, QVector<QLineF> lines, QVector<QRectF*> rects)
{
    for (int i = 0; i < lines.size(); i++)
    {
        for (int j = 0; j < lines.size(); j++)
        {
            if (i != j)
            {
                if((lines[i].y1() == lines[j].y1() && lines[i].y2() == lines[j].y2()) ||            //if y coords are the same
                        (lines[i].y1() == lines[j].y2() && lines[i].y2() == lines[j].y1()))
                {
                    QRectF cell;
                    if(lines[i].p1().y() == lines[j].p1().y())
                    {
                        cell = QRectF(lines[i].p1(), lines[j].p2());
                    } else {
                        cell = QRectF(lines[i].p1(),lines[j].p1());
                    }
                    cell = cell.normalized();
                    if(noCollide(rects, *cells, cell)) cells->push_back(cell);
                }
            }
        }
    }
}

bool PathFinder::noCollide(QVector<QRectF*> rects, QVector<QRectF> cells, QRectF cell)
{
    bool no_collide = true;
    if (cells.size() == 0) return true;
    else
    {
        for (int i = 0; i < cells.size(); i++)
        {
            if (cell.contains(cells[i]) || cell.intersects(cells[i])) no_collide = false;
        }
        for(int i = 0; i < rects.size(); i++)
        {
            if(cell.contains(*rects[i]) || cell.intersects(*rects[i])) no_collide = false;
        }
    }
    return no_collide;
}

void PathFinder::removeExtraneous(QVector<QRectF> *cells)
{
    for(int i = 0; i < cells->size(); i++)
    {
        if (cells->at(i).isEmpty())
        {
            cells->remove(i);
            i--;
        }
        else
        {
            for(int j = i+1; j < cells->size(); j++)
            {
                if (cells->at(i).topLeft() ==  cells->at(j).topLeft() &&            //are they the same rectangle?
                        cells->at(i).bottomRight() == cells->at(j).bottomRight())
                {
                    cells->remove(j);
                    i--;
                }
            }
        }
    }
}
