//
// Created by thomg on 9/03/2022.
//

#ifndef ENGINE_FUNCTIES_H
#define ENGINE_FUNCTIES_H

#include "draw2DLines.h"
#include "Figure.h"
#include "vector3d.h"
#include "l_parser.h"


Color VecToColor(vector<double> punten);

Lines2D drawLSystem(const LParser::LSystem2D &l_system, Color);

double GradToRad(double a);



Matrix scaleFigure(const double scale);

Matrix rotateX(const double angle);

Matrix rotateY(const double angle);

Matrix rotateZ(const double angle);

Matrix translate(const Vector3D &vector);

Matrix allTrans(const double scale, const double xangle, const double yangle,
const double zangle, const Vector3D &vector);

void applyTransformation(Figure &fig, const Matrix &m);


void toPolar(const Vector3D &point,double &theta, double &phi, double &r);

Matrix eyePointTrans(const Vector3D &eyepoint);

void applyTransformation(Figures3D &figs, const Matrix &m);

Point2D doProjection(const Vector3D &point, const double d);

Lines2D doProjection(const Figures3D &fig);

Vector3D VecToVec3d(vector<double> center);



#endif //ENGINE_FUNCTIES_H
