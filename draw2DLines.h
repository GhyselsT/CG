//
// Created by thomg on 2/03/2022.
//

#ifndef ENGINE_DRAW2DLINES_H
#define ENGINE_DRAW2DLINES_H

#include <iostream>
#include "Line2D.h"
#include "easy_image.h"
#include "cmath"
#include "Functies.h"
#include "ZBuffer.h"


using namespace std;

class draw2DLines {
public:
    img::EasyImage drawlines (Lines2D &lines, const int size, Color color, const string& type);

    pair<double,double> getimageXimageY(const int &size, double& xrange, double& yrange);
    double get_d(double& imagex,double& xrange);
    pair<double,double> getDCxDCy(double& d, double& xmin, double& xmax, double& ymin,double& ymax);
    pair<double,double> getdxdy(double& imagex, double& imagey, double& DCx, double& DCy);
    pair<double,double> getxmaxmin(Lines2D& lines);
    pair<double,double> getymaxmin(Lines2D& lines);
    pair<double,double> getRange(double& xmax, double & xmin, double & ymax, double & ymin);
};


#endif //ENGINE_DRAW2DLINES_H
