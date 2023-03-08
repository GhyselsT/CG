#include "easy_image.h"
#include "ini_configuration.h"

#include "draw2DLines.h"
#include "Line2D.h"
#include "Point2D.h"

#include "Functies.h"
#include "vector3d.h"

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
        return lines.drawlines(lsystem, size, backgroundcolor);
    }
    //Wireframe
    if (type == "Wireframe"){
        ///teken het wireframe
        tuple<double> eye = configuration["General"]["eye"];
        int nrFigures = configuration["General"]["nrFigures"];

        string wireframetype = configuration["Figure0"]["type"];
        double scale = configuration["Figure0"]["scale"];
        double xangle = configuration["Figure0"]["rotateX"];
        double yangle = configuration["Figure0"]["rotatey"];
        double zangle = configuration["Figure0"]["rotatez"];
        vector<double> center = configuration["Figure0"]["center"];

        Color color = VecToColor(configuration["Figure0"]["color"]);
        int nrPoints = configuration["Figure0"]["nrPoints"];
        int nrLines = configuration["Figure0"]["nrLines"];

        Figure figuur;
        for (int i = 0; i < nrPoints; ++i) {
            string s_2 = to_string(i);
            vector<double> p = configuration["Figure0"]["Point"+s_2];

            auto pvec = Vector3D::point(p[0],p[1],p[2]);
            figuur.points.emplace_back(pvec);
        }

        for (int i = 0; i < nrLines; ++i) {
            Face f;
            string s_2 = to_string(i);
            f.point_indexes = configuration["Figure0"]["Line"+s_2];
            figuur.faces.push_back(f);
        }

    }
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
