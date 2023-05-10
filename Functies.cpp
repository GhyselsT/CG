//
// Created by thomg on 9/03/2022.
//


#define _USE_MATH_DEFINES
#include "Functies.h"
#include "cmath"

#include <stack>


Color VecToColor(vector<double> punten){
    Color kleuren{};
    kleuren.red = punten[0];
    kleuren.green = punten[1];
    kleuren.blue = punten[2];
    return kleuren;
}

double GradToRad(double a){
    return (a/180)*M_PI;
}

img::Color toImgColor(const Color& kleur){
    img::Color k;
    k.red = lround(kleur.red * 255);
    k.blue = lround(kleur.blue * 255);
    k.green = lround(kleur.green * 255);
    return k;
}

Lines2D drawLSystem(const LParser::LSystem2D &l_system, Color color){
    string initiator = l_system.get_initiator();
    unsigned int iterations = l_system.get_nr_iterations();
    string draw;
    //looped door het het aantal iterations
    if (iterations == 0){
        draw = initiator;
    }
    else {
        for (int i = 1; i <= iterations; ++i) {
            draw = "";
            //looped door de initiator string
            for (char j: initiator) {
                if (j == '+') {
                    draw.append("+");
                } else if (j == '-') {
                    draw.append("-");
                } else if (j == '(') {
                    draw.append("(");
                } else if (j == ')') {
                    draw.append(")");
                } else {
                    draw.append(l_system.get_replacement(j));
                }
            }
            initiator = draw;
        }
    }
    //cout << draw << endl;

    double startAngle = l_system.get_starting_angle();
    double angle = l_system.get_angle();

    Point2D punt1{};
    punt1.x = 0;
    punt1.y = 0;

    Point2D punt2{};
    punt2.x = 0;
    punt2.y = 0;

    Line2D lijn{};
    Lines2D listLijnen;

    //lsystem haakjes
    stack<Point2D> savePunt;
    stack<double> saveHoek;

    for(char k : draw){
        if(k =='+'){
            startAngle += angle;
        }
        else if(k == '-'){
            startAngle -= angle;
        }
        else if(k =='('){
            savePunt.push(punt1);
            saveHoek.push(startAngle);
        }
        else if (k == ')'){
            punt1 = savePunt.top();
            savePunt.pop();
            startAngle = saveHoek.top();
            saveHoek.pop();
        }
        else{
            if(l_system.draw(k)){
                punt2.x = punt1.x + cos(GradToRad(startAngle));
                punt2.y = punt1.y + sin(GradToRad(startAngle));
                lijn.p1 = punt1;
                lijn.p2 = punt2;
                lijn.color = color;
                listLijnen.push_back(lijn);
            }
            else {
                punt2.x = punt1.x + cos(GradToRad(startAngle));
                punt2.y = punt1.y + sin(GradToRad(startAngle));
            }
            punt1 = punt2;
        }

    }
    return listLijnen;
}

Matrix scaleFigure(const double scale) {
    Matrix m;
    m(1,1) = scale;
    m(2,2) = scale;
    m(3,3) = scale;
    m(4,4) = 1;
    return m;
}

Matrix rotateX(const double angle) {
    Matrix m;
    double hoek = GradToRad(angle);
    //constants
    m(1,1) = 1;
    m(4,4) = 1;
    //angles
    m(2,2) = cos(hoek);
    m(2,3) = sin(hoek);
    m(3,2) = -sin(hoek);
    m(3,3) = cos(hoek);

    return m;
}

Matrix rotateY(const double angle) {
    Matrix m;
    double hoek = GradToRad(angle);
    //constants
    m(2,2) = 1;
    m(4,4) = 1;
    //angles
    m(1,1) = cos(hoek);
    m(1,3) = -sin(hoek);
    m(3,1) = sin(hoek);
    m(3,3) = cos(hoek);

    return m;
}

Matrix rotateZ(const double angle) {
    Matrix m;
    double hoek = GradToRad(angle);
    //constants
    m(3,3) = 1;
    m(4,4) = 1;
    //angles
    m(1,1) = cos(hoek);
    m(1,2) = sin(hoek);
    m(2,1) = -sin(hoek);
    m(2,2) = cos(hoek);

    return m;
}

Matrix translate(const Vector3D &vector) {
    Matrix m;
    //constants
    m(1,1) = 1;
    m(2,2) = 1;
    m(3,3) = 1;
    m(4,4) = 1;
    //vector
    m(4,1) = vector.x;
    m(4,2) = vector.y;
    m(4,3) = vector.z;

    return m;
}

Matrix allTrans(const double scale, const double xangle, const double yangle, const double zangle, const Vector3D &vector) {
    Matrix m;
    m = scaleFigure(scale) * rotateX(xangle) * rotateY(yangle) * rotateZ(zangle) * translate(vector);
    return m;
}

void applyTransformation(Figure &fig, const Matrix &m) {
    for (auto& punt:fig.points) {
        punt *= m;
    }
}


void toPolar(const Vector3D &point,double &theta, double &phi, double &r) {
    r = sqrt(pow(point.x,2) + pow(point.y,2) + pow(point.z,2));
    theta = atan2(point.y,point.x);
    phi = acos(point.z/r);
}

Matrix eyePointTrans(const Vector3D &eyepoint) {
    double theta;
    double phi;
    double r;
    toPolar(eyepoint, theta, phi, r);
    Matrix m;
    m(1,1) = -(sin(theta));
    m(1,2) = -(cos(theta)) * cos(phi);
    m(1,3) = cos(theta) * sin(phi);

    m(2,1) = cos(theta);
    m(2,2) = -(sin(theta)) * cos(phi);
    m(2,3) = sin(theta) * sin(phi);

    m(3,2) = sin(phi);
    m(3,3) = cos(phi);

    m(4,3) = -r;
    m(4,4) = 1;

    return m;
}

void applyTransformation(Figures3D &figs, const Matrix &m) {
    for (auto &item:figs) {
        applyTransformation(item,m);
    }
}

Point2D doProjection(const Vector3D &point, const double d) {
    Point2D punt{};
    punt.x = (d * point.x)/-point.z;
    punt.y = (d * point.y)/-point.z;
    return punt;
}


Lines2D doProjection(const Figures3D &fig) {
    Lines2D lijnen;
    int ctr = 0;
    for (auto item:fig) {
        for (auto i:item.faces) {
            for (int p = 0; p < i.point_indexes.size(); ++p) {
                Line2D lijn{};
                int f = i.point_indexes[p];
                int s = i.point_indexes[(p+1)%i.point_indexes.size()];
                lijn.p1 = doProjection(item.points[f], 1);
                lijn.p2 = doProjection((item.points[s]), 1);
                lijn.z1 = item.points[f].z;
                lijn.z2 = item.points[s].z;
                lijn.color = item.color;
                lijnen.emplace_back(lijn);
                ctr +=1;
                //cout << ctr <<endl;
                //cout << lijn.p1.x <<"  x val   " << lijn.p1.y << "  y val " << endl;
            }
        }
    }
    return lijnen;
}

Vector3D VecToVec3d(vector<double> center) {
    Vector3D output = Vector3D::point(0,0,0);
    output.x = center[0];
    output.y = center[1];
    output.z = center[2];
    return output;
}

vector<Face> triangulate(const Face &face) {
    vector<Face> output;
    //cout << face.point_indexes.size() << endl;
    if (face.point_indexes.size() <= 3){
        output.emplace_back(Face({face.point_indexes[0],face.point_indexes[1],face.point_indexes[2]}));
    }
    else{
        for (int i = 1; i <= face.point_indexes.size()-2; ++i) {
            //punt 0,punt zelf, volgend punt
            output.emplace_back(Face({face.point_indexes[0],face.point_indexes[i],face.point_indexes[i+1]}));

        }
    }
    return output;
}

void generateFractal(const Figure &fig, Figures3D &fractal, const int nr_iterations, double scale) {

    if (nr_iterations == 0){
        fractal.push_back(fig);
        return;
    }

    int ctr = 0;
    for (auto &point: fig.points) {
        Figure tempfig = fig;
        auto M = scaleFigure(1/scale);
        applyTransformation(tempfig, M);

        auto T = translate(point - tempfig.points[ctr] );
        applyTransformation(tempfig, T);

//      fractal.push_back(tempfig);
        ctr +=1;
        generateFractal(tempfig,fractal,nr_iterations-1,scale);
    }

}












