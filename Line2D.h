//
// Created by thomg on 2/03/2022.
//

#ifndef ENGINE_LINE2D_H
#define ENGINE_LINE2D_H

#include "Color.h"
#include "Point2D.h"

#include <list>

using namespace std;

class Line2D {
public:
    Point2D p1;
    Point2D p2;
    Color color;
};
using Lines2D = list<Line2D>;


#endif //ENGINE_LINE2D_H
