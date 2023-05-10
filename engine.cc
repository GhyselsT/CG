#include "easy_image.h"
#include "ini_configuration.h"

#include "draw2DLines.h"
#include "Line2D.h"
#include "Point2D.h"

#include "Functies.h"
#include "vector3d.h"
#include "3DFiguren.h"
#include "Face.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>



img::EasyImage generate_image(const ini::Configuration &configuration) {
    ///2D lijn tekenen
    /*
    Lines2D lijnen;
    Line2D lijn;
    lijn.p1.x = 1;
    lijn.p1.y = 1;

    lijn.p2.x = 5;
    lijn.p2.y = 5;

    lijn.color.red = 1;
    lijn.color.blue = 0;
    lijn.color.green = 0;

    Line2D lijna;
    lijna.p1.x = 0;
    lijna.p1.y = 8;

    lijna.p2.x = 5;
    lijna.p2.y = 5;

    lijna.color.red = 0;
    lijna.color.blue = 0;
    lijna.color.green = 1;

    lijnen.push_back(lijn);
    lijnen.push_back(lijna);

    draw2DLines teken;
    return teken.drawlines(lijnen,500);
    */


    //system
    string type = configuration["General"]["type"].as_string_or_die();
    int size = configuration["General"]["size"].as_int_or_die();
    Color backgroundcolor = VecToColor(configuration["General"]["backgroundcolor"].as_double_tuple_or_die());

    //2DLSysytem
    if (type == "2DLSystem") {
        //teken het 2d l-systeem
        string inputfile = configuration["2DLSystem"]["inputfile"];
        Color color = VecToColor(configuration["2DLSystem"]["color"]);
        LParser::LSystem2D LSystem2D;
        ifstream file(inputfile);
        file >> LSystem2D;
        file.close();

        draw2DLines lines;
        auto lsystem = drawLSystem(LSystem2D, color);
        return lines.drawlines(lsystem, size, backgroundcolor, type);
    }
    //Wireframe
    if (type == "Wireframe" || type == "ZBufferedWireframe" || type == "ZBuffering"){
        ///teken het wireframe
        vector<double> eye = configuration["General"]["eye"];
        int nrFigures = configuration["General"]["nrFigures"];
        Figures3D figlist;
        for (int i = 0; i < nrFigures; ++i) {
            Figure figuur;
            string endf = to_string(i);
            string wireframetype = configuration["Figure" + endf]["type"];
            double scale = configuration["Figure" + endf]["scale"];
            double xangle = configuration["Figure" + endf]["rotateX"];
            double yangle = configuration["Figure" + endf]["rotateY"];
            double zangle = configuration["Figure" + endf]["rotateZ"];
            vector<double> center = configuration["Figure" + endf]["center"];

            Color color = VecToColor(configuration["Figure" + endf]["color"]);

            if (wireframetype == "Cube") {
                figuur = createKubus();
                figuur.color = color;
                auto m = allTrans(scale, xangle, yangle, zangle, VecToVec3d(center));
                applyTransformation(figuur, m);
                figlist.emplace_back(figuur);
            }
            else if(wireframetype == "Tetrahedron"){
                figuur = createTetrahedron();
                figuur.color = color;
                auto m = allTrans(scale, xangle, yangle, zangle, VecToVec3d(center));
                applyTransformation(figuur, m);
                figlist.emplace_back(figuur);
            }
            else if (wireframetype == "Octahedron"){
                figuur = createOctahedron();
                figuur.color = color;
                auto m = allTrans(scale, xangle, yangle, zangle, VecToVec3d(center));
                applyTransformation(figuur, m);
                figlist.emplace_back(figuur);
            }
            else if(wireframetype == "Icosahedron"){
                figuur = createIcosahedron();
                figuur.color = color;
                auto m = allTrans(scale, xangle, yangle, zangle, VecToVec3d(center));
                applyTransformation(figuur, m);
                figlist.emplace_back(figuur);
            }
            else if(wireframetype == "Dodecahedron"){
                figuur = createDodecahedron();
                figuur.color = color;
                auto m = allTrans(scale, xangle, yangle, zangle, VecToVec3d(center));
                applyTransformation(figuur, m);
                figlist.emplace_back(figuur);
            }
            else if(wireframetype == "Sphere"){
                const int n = configuration["Figure"+endf]["n"];
                figuur = createSphere(1.0,n);
                figuur.color = color;
                auto m = allTrans(scale, xangle, yangle, zangle, VecToVec3d(center));
                applyTransformation(figuur, m);
                figlist.emplace_back(figuur);
            }
            else if(wireframetype == "Cone"){
                const double h = configuration["Figure"+endf]["height"];
                const int n = configuration["Figure"+endf]["n"];
                figuur = createCone(n,h);
                figuur.color = color;
                auto m = allTrans(scale, xangle, yangle, zangle, VecToVec3d(center));
                applyTransformation(figuur, m);
                figlist.emplace_back(figuur);
            }
            else if (wireframetype == "Cylinder"){
                const double h = configuration["Figure"+endf]["height"];
                const int n = configuration["Figure"+endf]["n"];
                figuur = createCylinder(n,h);
                figuur.color = color;
                auto m = allTrans(scale, xangle, yangle, zangle, VecToVec3d(center));
                applyTransformation(figuur, m);
                figlist.emplace_back(figuur);
            }
            else if(wireframetype == "Torus"){
                const double r = configuration["Figure" + endf]["r"];
                const double R = configuration["Figure" + endf]["R"];
                const int n = configuration["Figure" + endf]["n"];
                const int m = configuration["Figure" + endf]["m"];
                figuur = createTorus(r,R,n,m);
                figuur.color = color;
                auto v = allTrans(scale, xangle, yangle, zangle, VecToVec3d(center));
                applyTransformation(figuur, v);
                figlist.emplace_back(figuur);
            }
            else if(wireframetype == "3DLSystem"){
                string inputfile = configuration["Figure"+endf]["inputfile"];
                LParser::LSystem3D LSystem3D;
                ifstream file(inputfile);
                file >> LSystem3D;
                file.close();

                figuur = draw3Dlsystem(LSystem3D);
                figuur.color = color;
                auto v = allTrans(scale, xangle, yangle, zangle, VecToVec3d(center));
                applyTransformation(figuur, v);
                figlist.emplace_back(figuur);

            }
            else if(wireframetype == "FractalCube"){
                Figures3D fractals;
                figuur = createKubus();
                figuur.color = color;

                const int nrIterations = configuration["Figure"+endf]["nrIterations"];
                double fractalScale = configuration["Figure"+endf]["fractalScale"];
                generateFractal(figuur,fractals,nrIterations,fractalScale);

                for (auto& fig:fractals) {
                    auto m = allTrans(scale, xangle, yangle, zangle, VecToVec3d(center));
                    applyTransformation(fig, m);
                    figlist.emplace_back(fig);
                }
            }
            else if (wireframetype == "FractalTetrahedron"){
                Figures3D fractals;
                figuur = createTetrahedron();
                figuur.color = color;
                const int nrIterations = configuration["Figure"+endf]["nrIterations"];
                double fractalScale = configuration["Figure"+endf]["fractalScale"];
                generateFractal(figuur,fractals,nrIterations,fractalScale);
                for (auto& fig:fractals) {
                    auto m = allTrans(scale, xangle, yangle, zangle, VecToVec3d(center));
                    applyTransformation(fig, m);
                    figlist.emplace_back(fig);
                }
            }
            else if(wireframetype == "FractalOctahedron"){
                Figures3D fractals;
                figuur = createOctahedron();
                figuur.color = color;
                const int nrIterations = configuration["Figure"+endf]["nrIterations"];
                double fractalScale = configuration["Figure"+endf]["fractalScale"];
                generateFractal(figuur,fractals,nrIterations,fractalScale);
                for (auto& fig:fractals) {
                    auto m = allTrans(scale, xangle, yangle, zangle, VecToVec3d(center));
                    applyTransformation(fig, m);
                    figlist.emplace_back(fig);
                }
            }
            else if (wireframetype =="FractalIcosahedron"){
                Figures3D fractals;
                figuur = createIcosahedron();
                figuur.color = color;
                const int nrIterations = configuration["Figure"+endf]["nrIterations"];
                double fractalScale = configuration["Figure"+endf]["fractalScale"];
                generateFractal(figuur,fractals,nrIterations,fractalScale);
                for (auto& fig:fractals) {
                    auto m = allTrans(scale, xangle, yangle, zangle, VecToVec3d(center));
                    applyTransformation(fig, m);
                    figlist.emplace_back(fig);
                }
            }
            else if (wireframetype == "FractalDodecahedron"){
                Figures3D fractals;
                figuur = createDodecahedron();
                figuur.color = color;
                const int nrIterations = configuration["Figure"+endf]["nrIterations"];
                double fractalScale = configuration["Figure"+endf]["fractalScale"];
                generateFractal(figuur,fractals,nrIterations,fractalScale);
                for (auto& fig:fractals) {
                    auto m = allTrans(scale, xangle, yangle, zangle, VecToVec3d(center));
                    applyTransformation(fig, m);
                    figlist.emplace_back(fig);
                }
            }

            //random dink tekene
            else {
                figuur.color = color;
                int nrPoints = configuration["Figure" + endf]["nrPoints"];
                int nrLines = configuration["Figure" + endf]["nrLines"];
                for (int p = 0; p < nrPoints; ++p) {
                    string s_2 = to_string(p);
                    vector<double> pt = configuration["Figure" + endf]["point" + s_2];
                    auto pvec = Vector3D::point(pt[0], pt[1], pt[2]);
                    figuur.points.emplace_back(pvec);
                }
                for (int l = 0; l < nrLines; ++l) {
                    Face f;
                    string s_2 = to_string(l);
                    f.point_indexes = configuration["Figure" + endf]["line" + s_2];
                    figuur.faces.push_back(f);
                }
                auto m = allTrans(scale, xangle, yangle, zangle, VecToVec3d(center));
                applyTransformation(figuur, m);
                figlist.emplace_back(figuur);

            }

        }


        draw2DLines teken;

        if (type == "ZBufferedWireframe") {
            auto v = eyePointTrans(VecToVec3d(eye));
            applyTransformation(figlist,v);auto pp = doProjection(figlist);
            return teken.drawlines(pp, size, backgroundcolor, type);
        }
        if (type == "ZBuffering"){
            auto v = eyePointTrans(VecToVec3d(eye));
            applyTransformation(figlist,v);
            for (auto& figuur:figlist) {
                // lijst van faces
                vector<Face> facelist;
                for(auto& f:figuur.faces){
                    auto newf = triangulate(f);
                    for(auto newff: newf){
                        facelist.push_back(newff);
                    }
                   // facelist.insert(facelist.begin(), newf.begin(), newf.end());

                }
                // stel figuur.faces gelijk aan lijst
                figuur.faces = facelist;
            }

            //cout <<"figlist size" << figlist.size() << endl;
            auto lines = doProjection(figlist);
            auto xvals = teken.getxmaxmin(lines);
            auto yvals = teken.getymaxmin(lines);

            auto ranges = teken.getRange(xvals.first,xvals.second,yvals.first,yvals.second);
            auto images = teken.getimageXimageY(size,ranges.first, ranges.second);

            auto d = teken.get_d(images.first,ranges.first);
            auto DCs = teken.getDCxDCy(d,xvals.second,xvals.first,yvals.second,yvals.first);
            auto dxdy = teken.getdxdy(images.first,images.second,DCs.first,DCs.second);

//            cout << images.first << endl;
//            cout << images.second << endl;
            img::EasyImage image(images.first,images.second);
            ZBuffer zbuf = ZBuffer(lround(images.first), lround(images.second));
            for (auto& fig:figlist) {
                for (auto& face:fig.faces) {
                    auto A = Vector3D::vector( fig.points[face.point_indexes[0]]);
                    auto B = Vector3D::vector( fig.points[face.point_indexes[1]]);
                    auto C = Vector3D::vector( fig.points[face.point_indexes[2]]);
                    zbuf.draw_zbuf_triag(zbuf,image,A,B,C,d,dxdy.first,dxdy.second,fig.color);
                }
            }

            return image;

        }
        else{
            auto v = eyePointTrans(VecToVec3d(eye));
            applyTransformation(figlist,v);
            auto pp = doProjection(figlist);

            return teken.drawlines(pp, size, backgroundcolor, type);
        }
    }
    return img::EasyImage();
}

int main(int argc, char const *argv[]) {
    int retVal = 0;
    try {
        std::vector<std::string> args = std::vector<std::string>(argv + 1, argv + argc);
        if (args.empty()) {
            std::ifstream fileIn("filelist");
            std::string filelistName;
            while (std::getline(fileIn, filelistName)) {
                args.push_back(filelistName);
            }
        }
        for (std::string fileName: args) {
            ini::Configuration conf;
            try {
                std::ifstream fin(fileName);
                fin >> conf;
                fin.close();
            }
            catch (ini::ParseException &ex) {
                std::cerr << "Error parsing file: " << fileName << ": " << ex.what() << std::endl;
                retVal = 1;
                continue;
            }

            img::EasyImage image = generate_image(conf);
            if (image.get_height() > 0 && image.get_width() > 0) {
                std::string::size_type pos = fileName.rfind('.');
                if (pos == std::string::npos) {
                    //filename does not contain a '.' --> append a '.bmp' suffix
                    fileName += ".bmp";
                } else {
                    fileName = fileName.substr(0, pos) + ".bmp";
                }
                try {
                    std::ofstream f_out(fileName.c_str(), std::ios::trunc | std::ios::out | std::ios::binary);
                    f_out << image;

                }
                catch (std::exception &ex) {
                    std::cerr << "Failed to write image to file: " << ex.what() << std::endl;
                    retVal = 1;
                }
            } else {
                std::cout << "Could not generate image for " << fileName << std::endl;
            }
        }
    }
    catch (const std::bad_alloc &exception) {
        //When you run out of memory this exception is thrown. When this happens the return value of the program MUST be '100'.
        //Basically this return value tells our automated test scripts to run your engine on a pc with more memory.
        //(Unless of course you are already consuming the maximum allowed amount of memory)
        //If your engine does NOT adhere to this requirement you risk losing points because then our scripts will
        //mark the test as failed while in reality it just needed a bit more memory
        std::cerr << "Error: insufficient memory" << std::endl;
        retVal = 100;
    }
    return retVal;
}
