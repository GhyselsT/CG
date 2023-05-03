//
// Created by thomg on 2/03/2022.
//


#include "draw2DLines.h"

img::EasyImage draw2DLines::drawlines(Lines2D &lines, const int size, Color color, const string& type) {

    auto xvals = getxmaxmin(lines);
    double xmax = xvals.first;
    double xmin = xvals.second;

    auto yvalq = getymaxmin(lines);
    double ymax = yvalq.first;
    double ymin = yvalq.second;
// range van x en range van y berekenen
    auto ranges = getRange(xmax,xmin,ymax,ymin);
    double xrange = ranges.first;
    double yrange = ranges.second;
// achtergrond kleur
    img::Color color1;
    color1.red = lround(color.red * 255);
    color1.blue = lround(color.blue * 255);
    color1.green = lround(color.green * 255);

// image van x en image van y berekenen
    auto images = getimageXimageY(size,xrange,yrange);
    double imagex = images.first;
    double imagey = images.second;
// schaalfactor berekenenµ
    auto d = get_d(images.first,xrange);
//verschuif de tekening
    auto DCs = getDCxDCy(d,xmin,xmax,ymin,ymax);
    auto dxy = getdxdy(images.first,images.second,DCs.first,DCs.second);
    double dx = dxy.first;
    double dy = dxy.second;

    //cout << imagex << " imagex" << "\n";
    //cout << imagey << " imagey" << "\n";
    img::EasyImage ima = img::EasyImage (lround(imagex), lround(imagey),color1);
    auto zbuf = ZBuffer(lround(imagex), lround(imagey));





// coordinaten vermendigvuldigen met de schaalfactor
    for(auto &line:lines){
        line.p1.x = line.p1.x * d;
        line.p1.y = line.p1.y * d;
        line.p2.x = line.p2.x * d;
        line.p2.y = line.p2.y * d;
    }



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
    for(auto &line:lines) {
        img::Color linecolor = toImgColor(line.color);

        //img::Color kleur;
        //kleur.red = lround(line.color.red * 255);
        //kleur.blue = lround(line.color.blue * 255);
        //kleur.green = lround(line.color.green * 255);
        //cout << (int)kleur.red << " " << (int)kleur.blue << " " << (int)kleur.green << endl;
        if (type == "ZBufferedWireframe") {
            zbuf.draw_zbuff_line(zbuf,ima,lround(line.p1.x), lround(line.p1.y),line.z1, lround(line.p2.x), lround(line.p2.y),line.z2,line.color);
        }
        else{
            //std::cout << line.p1.x << ", " << line.p1.y << " -> " << line.p2.x << ", "<< line.p2.y << std::endl;
            ima.draw_line(lround(line.p1.x), lround(line.p1.y), lround(line.p2.x), lround(line.p2.y), linecolor);
        }
    }
    return ima;
}

pair<double, double> draw2DLines::getimageXimageY(const int& size, double& xrange, double& yrange) {
    pair<double,double>output;
    output.first = size * xrange/max(xrange,yrange);
    output.second = size * yrange/max(xrange,yrange);
    return output;
}

double draw2DLines::get_d(double &imagex, double &xrange) {
    return 0.95 * (imagex/xrange);
}

pair<double,double> draw2DLines::getDCxDCy(double &d, double &xmin, double &xmax, double &ymin, double &ymax) {
    pair<double,double>output;
    output.first = d * (xmin + xmax)/2.0;
    output.second = d * (ymin + ymax)/2.0;
    return output;
}

pair<double, double> draw2DLines::getdxdy(double &imagex, double &imagey, double &DCx, double &DCy) {
    pair<double,double> output;
    output.first = (imagex/2.0) - DCx;
    output.second = (imagey/2.0) - DCy;
    return output;
}

pair<double, double> draw2DLines::getxmaxmin(Lines2D &lines) {
    pair<double,double> xvals;
    xvals.first = lines.back().p1.x;
    xvals.second = lines.front().p1.x;

//Max coordinaten berekenen door een for loop
    for(auto i:lines) {
        if (i.p1.x < xvals.second) {
            xvals.second = i.p1.x;
        }
        if (i.p2.x < xvals.second) {
            xvals.second = i.p2.x;
        }
        if (i.p1.x > xvals.first) {
            xvals.first = i.p1.x;
        }
        if (i.p2.x > xvals.first) {
            xvals.first = i.p2.x;
        }
    }
    return xvals;
}

pair<double, double> draw2DLines::getymaxmin(Lines2D &lines) {
    pair<double,double> yvals;
    yvals.first = lines.back().p1.y;
    yvals.second = lines.front().p1.y;

//Max coordinaten berekenen door een for loop
    for(auto i:lines) {
        if (i.p1.y < yvals.second) {
            yvals.second = i.p1.y;
        }
        if (i.p2.y < yvals.second) {
            yvals.second = i.p2.y;
        }
        if (i.p1.y > yvals.first) {
            yvals.first = i.p1.y;
        }
        if (i.p2.y > yvals.first) {
            yvals.first = i.p2.y;
        }
    }
    return yvals;
}

pair<double, double> draw2DLines::getRange(double &xmax, double &xmin, double &ymax, double &ymin) {
    pair<double,double> out;
    out.first = xmax - xmin;
    out.second = ymax - ymin;
    return out;
}
