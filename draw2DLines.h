//
// Created by thomg on 2/03/2022.
//

#ifndef ENGINE_DRAW2DLINES_H
#define ENGINE_DRAW2DLINES_H

#include <iostream>
#include "Line2D.h"
#include "easy_image.h"

using namespace std;

class draw2DLines {
public:
    img::EasyImage drawlines (Lines2D &lines, const int size, Color color);

};


#endif //ENGINE_DRAW2DLINES_H
