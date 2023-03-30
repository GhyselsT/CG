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


