//
// Created by thomg on 29/03/2023.
//

#ifndef ENGINE_ZBUFFER_H
#define ENGINE_ZBUFFER_H

#include <vector>
#include <limits>
#include "easy_image.h"
#include "Color.h"
#include <iostream>

#include "algorithm"
#include <assert.h>
#include "cmath"
#include "Functies.h"


using namespace std;

class ZBuffer :public vector<vector<double>>{
public:
    ZBuffer(const int width, const int height);

    void draw_zbuff_line(ZBuffer& zbuf, img::EasyImage& image,
                         unsigned int x0,unsigned int y0,
                         double z0,
                         unsigned int x1,unsigned int y1,
                         double z1,
                         const Color& color);
    
};


#endif //ENGINE_ZBUFFER_H
