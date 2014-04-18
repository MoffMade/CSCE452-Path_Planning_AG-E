#include "pathfinder.h"

static QVector<QPoint> pathFinder(QRectF window,QRectF r1, QRectF r2, QRectF r3, QPoint start, QPoint goal)
{
    /*1. do a vertical cell decomposition
     *2. construct a graph of all the cells and their interconnections
     *3. use some random weird pathfinding algorithm on the graph
     *4. generate and return QLines for each node, representing the path*/

    /*Step 1
     *
     * 1. sort by x coordinates of the points
     * 2. draw Qlines from each vertical face of each QRectF.
     * 3. two lines with the same y coordinates define a cell
     * 4. check for and remove overlaps
     * 5. add Qlines which traverse whole vertical face of each QRectF.
     * 6. repeat 3 and 4
     * 7. DONE :)
     * /
}
