//
// Created by thomg on 29/03/2023.
//

#include "ZBuffer.h"


ZBuffer::ZBuffer(const int width, const int height) {
    //cout << width <<" " <<height << endl;
    for (int i = 0; i < width; ++i) {
        vector<double> rij;
        for (int j = 0; j < height; ++j) {
            rij.push_back(numeric_limits<double>::infinity());
        }
        this->push_back(rij);
    }
}

void ZBuffer::draw_zbuff_line(ZBuffer &zbuf, img::EasyImage &image, unsigned int x0, unsigned int y0,
                               double z0, unsigned int x1, unsigned int y1, double z1,
                               const Color &color) {
    assert(x0 < image.get_width() && y0 < image.get_height());
    assert(x1 < image.get_width() && y1 < image.get_height());
    if (x0 == x1)
    {
        auto zmin = (std::min(y0, y1) == y0) ? z0:z1;
        auto zmax = (std::max(y0, y1) == y0) ? z0:z1;
        //special case for x0 == x1
        for (unsigned int i = std::min(y0, y1); i <= std::max(y0, y1); i++)
        {
            double z;
            if(y0 == y1){
                z = std::min(y0, y1) * (zmax - zmin);
            } else{
                z = zmin + (zmax - zmin)*(i - std::min(y0, y1))/(std::max(y0, y1) - std::min(y0, y1));
            }
            double eoz = 1/z;
            if (eoz < zbuf[x0][i]){
                (image)(x0, i) = toImgColor(color);
                zbuf[x0][i] = eoz;
            }

        }
    }
    else if (y0 == y1)
    {
        auto zmin = (std::min(x0, x1) == x0) ? z0:z1;
        auto zmax = (std::max(x0, x1) == x0) ? z0:z1;
        //special case for y0 == y1
        for (unsigned int i = std::min(x0, x1); i <= std::max(x0, x1); i++)
        {
            auto z = zmin + (zmax - zmin) * (i - std::min(x0, x1))/(std::max(x0, x1) - std::min(x0, x1));
            double eoz = 1/z;

            if (eoz < zbuf[i][y0]){
                (image)(i,y0) = toImgColor(color);
                zbuf[i][y0] = eoz;
            }
        }
    }
    else
    {
        if (x0 > x1)
        {
            //flip points if x1>x0: we want x0 to have the lowest value
            std::swap(x0, x1);
            std::swap(y0, y1);
            std::swap(z0, z1);
        }
        double m = ((double) y1 - (double) y0) / ((double) x1 - (double) x0);
        if (-1.0 <= m && m <= 1.0)
        {
            double a = (x1-x0);
            for (unsigned int i = 0; i <= (x1 - x0); i++)
            {
                double p = (a-i)/a;
                double eoz = p/z0 + (1-p)/z1;

                //cout <<  p<< " " <<eoz << " "  << "Z0 " << 1/z0 << " Z1 " << 1/z1 << endl;
                if(eoz < zbuf[x0 + i][ (unsigned int) lround(y0 + m * i)]){
                    (image)(x0 + i, (unsigned int) lround(y0 + m * i)) = toImgColor(color);
                    zbuf[x0 + i][ (unsigned int) lround(y0 + m * i)] = eoz;
                }
                //(image)(x0 + i, (unsigned int) lround(y0 + m * i)) = toImgColor(color);
            }
        }
        else if (m > 1.0)
        {
            double a = (y1 -y0);
            for (unsigned int i = 0; i <= (y1 - y0); i++) {
                double p = (a - i) / a;
                double eoz = p / z0 + (1 - p) / z1;

                if (eoz < zbuf[(unsigned int) lround(x0 + (i / m))][y0 + i]) {
                    (image)((unsigned int) lround(x0 + (i / m)), y0 + i) = toImgColor(color);
                    zbuf[(unsigned int) lround(x0 + (i / m))][y0 + i] = eoz;
                }
            }
        }
        else if (m < -1.0)
        {
            double a = (y0 - y1);
            for (unsigned int i = 0; i <= (y0 - y1); i++)
            {
                double p = (a - i) / a;
                double eoz = p / z0 + (1 - p) / z1;
                if (eoz < zbuf[(unsigned int) lround(x0 - (i / m))][ y0 - i]) {
                    (image)((unsigned int) lround(x0 - (i / m)), y0 - i) = toImgColor(color);
                    zbuf[(unsigned int) lround(x0 - (i / m))][ y0 - i] = eoz;
                }
            }
        }
    }

}

//dees werkende krijgen
void ZBuffer::draw_zbuf_triag(ZBuffer &zbuf, img::EasyImage &image,
                              const Vector3D &A, const Vector3D &B, const Vector3D &C,
                              double d, double dx, double dy,
                              const Color& color) {

    auto Aproj = doProjection(A,d);
    Aproj.x += dx;
    Aproj.y += dy;

    auto Bproj = doProjection(B,d);
    Bproj.x += dx;
    Bproj.y += dy;

    auto Cproj = doProjection(C,d);
    Cproj.x += dx;
    Cproj.y += dy;

    Vector3D u = B-A;
    Vector3D v = C-A;

    auto w1 = (u.y*v.z) - (u.z*v.y);
    auto w2 = (u.z*v.x) - (u.x*v.z);
    auto w3 = (u.x*v.y) - (u.y*v.x);
    auto w = Vector3D::point(w1,w2,w3);

    auto k = (w1*A.x) + (w2*A.y) + (w3*A.z);

    auto dzdx = (w1/(-d*k));
    auto dzdy = (w2/(-d*k));

    double Gx = (Aproj.x + Bproj.x + Cproj.x)/3;
    double Gy = (Aproj.y + Bproj.y + Cproj.y)/3;
    //double Gz = (3*A.z + 3*B.z + 3*C.z);
    double eoGz = 1/(3* A.z) + 1 / (3 * B.z) + 1 / (3 * C.z);

    auto minimum = (int)lround(min({Aproj.y,Bproj.y,Cproj.y})+0.5);
    auto maximum = (int) lround(max({Aproj.y,Bproj.y,Cproj.y})-0.5);
    for (int yi = minimum; yi <= maximum; ++yi) {
        double xLab, xLac,xLbc;
        xLab = numeric_limits<double>::infinity();
        xLac = numeric_limits<double>::infinity();
        xLbc = numeric_limits<double>::infinity();
        double xRab,xRac,xRbc;
        xRab = - numeric_limits<double>::infinity();
        xRac = - numeric_limits<double>::infinity();
        xRbc = - numeric_limits<double>::infinity();
        //AB
        if ((yi-Aproj.y)*(yi -Bproj.y) <= 0 && Aproj.y != Bproj.y){
            double xi = Bproj.x + (Aproj.x - Bproj.x)*((yi-Bproj.y)/(Aproj.y - Bproj.y));
            xLab = xi;
            xRab = xi;
        }
        if ((yi-Aproj.y)*(yi -Cproj.y) <= 0 && Aproj.y != Cproj.y) {
            double xi = Cproj.x + (Aproj.x - Cproj.x) * ((yi - Cproj.y) / (Aproj.y - Cproj.y));
            xLac = xi;
            xRac = xi;
        }
        if ((yi-Bproj.y)*(yi -Cproj.y) <= 0 && Bproj.y != Cproj.y) {
            double xi = Cproj.x + (Bproj.x - Cproj.x) * ((yi - Cproj.y) / (Bproj.y - Cproj.y));
            xLbc = xi;
            xRbc = xi;
        }

        int xL = (int) lround(min({xLab, xLac, xLbc}) + 0.5);
        int xR = (int) lround(max({xRab, xRac, xRbc}) - 0.5);

        for (int x = xL; x <= xR; ++x) {
            double eoz = ((1.0001)*(eoGz)) + (x - Gx)*dzdx + (yi - Gy)*dzdy;
            if (eoz < zbuf[x][yi]){
                //std::cout << "test" << std::endl;
                image(x,yi) = toImgColor(color);
                zbuf[x][yi] = eoz;
            }
        }

    }
}


