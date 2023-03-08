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

Lines2D drawLSystem(const LParser::LSystem2D &l_system, Color color){
    string initiator = l_system.get_initiator();
    unsigned int iterations = l_system.get_nr_iterations();
    string draw;
    //looped door het het aantal iterations
    for (int i = 1; i <= iterations; ++i) {
        draw = "";
        //looped door de initiator string
        for (char j : initiator) {
            if (j == '+'){
                draw.append("+");
            }
            else if (j == '-'){
                draw.append("-");
            }
            else if(j == '('){
                draw.append("(");
            }
            else if(j == ')'){
                draw.append(")");
            }
            else {
                draw.append(l_system.get_replacement(j));
            }
        }
        initiator = draw;
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
    m(1,1) *= scale;
    m(2,2) *= scale;
    m(3,3) *= scale;
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






