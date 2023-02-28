//
// Created by thomg on 2/03/2022.
//


#include "draw2DLines.h"
#include "cmath"

img::EasyImage draw2DLines::drawlines(Lines2D &lines, const int size, Color color) {

    double xmax = lines.front().p1.x;
    double xmin = lines.front().p1.x;
    double ymax = lines.front().p1.y;
    double ymin = lines.front().p1.y;

//Max coordinaten berekenen door een for loop
    for(auto &line:lines){
        if(xmax < line.p1.x){
            xmax = line.p1.x;
        }
        if(xmax < line.p2.x){
            xmax = line.p2.x;
        }
        if(ymax < line.p1.y){
            ymax = line.p1.y;
        }
        if(ymax < line.p2.y){
            ymax = line.p2.y;
        }
        if(xmin > line.p1.x){
            xmin = line.p1.x;
        }
        if(xmin > line.p2.x){
            xmin = line.p2.x;
        }
        if(ymin > line.p1.y){
            ymin = line.p1.y;
        }
        if(ymin > line.p2.y){
            ymin = line.p2.y;
        }
    }

// range van x en range van y berekenen
    double xrange = (xmax - xmin);
    double yrange = (ymax - ymin);

// achtergrond kleur
    img::Color color1;
    color1.red = lround(color.red * 255);
    color1.blue = lround(color.blue * 255);
    color1.green = lround(color.green * 255);

// image van x en image van y berekenen
    double imagex = size * ((xrange)/(max(xrange,yrange)));
    double imagey = size * ((yrange)/(max(xrange,yrange)));
    img::EasyImage ima = img::EasyImage (lround(imagex), lround(imagey),color1);

// schaalfactor berekenen
    double d = 0.95 * ((imagex)/(xrange));

// coordinaten vermendigvuldigen met de schaalfactor
    for(auto &line:lines){
        line.p1.x = line.p1.x * d;
        line.p1.y = line.p1.y * d;
        line.p2.x = line.p2.x * d;
        line.p2.y = line.p2.y * d;
    }

//verschuif de tekening
    double DCx = d * ((xmin + xmax)/2);
    double DCy = d * ((ymin + ymax)/2);
    double dx = (imagex/2) - DCx;
    double dy = (imagey/2) - DCy;

//Tel bij alle punten van de lijntekening (dx, dy) op.

    for(auto &line:lines){
        line.p1.x = line.p1.x + dx;
        line.p1.y = line.p1.y + dy;
        line.p2.x = line.p2.x + dx;
        line.p2.y = line.p2.y + dy;
    }

// Rond de coördinaten van de punten af, en teken de lijnen op de image
    for(auto &line:lines) {
        line.p1.x = lround(line.p1.x);
        line.p1.y = lround(line.p1.y);
        line.p2.x = lround(line.p2.x);
        line.p2.y = lround(line.p2.y);
    }

//kleuren van de lijn
    for(auto &line:lines){
        img::Color kleur;
        kleur.red = lround(line.color.red * 255);
        kleur.blue = lround(line.color.blue * 255);
        kleur.green = lround(line.color.green * 255);
        //cout << (int)kleur.red << " " << (int)kleur.blue << " " << (int)kleur.green << endl;
        ima.draw_line(line.p1.x, line.p1.y, line.p2.x, line.p2.y,kleur);
    }

    return ima;
}
