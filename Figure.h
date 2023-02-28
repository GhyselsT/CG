//
// Created by thomg on 11/05/2022.
//

#ifndef ENGINE_FIGURE_H
#define ENGINE_FIGURE_H

#include <iostream>
#include <vector>
#include "vector3d.h"
#include "Face.h"
#include "Color.h"
#include "list"

using namespace std;

class Figure {
public:
    vector<Vector3D> points;
    vector<Face> faces;
    Color color;
};
typedef list<Figure>Figures3D;


#endif //ENGINE_FIGURE_H
