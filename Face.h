//
// Created by thomg on 11/05/2022.
//

#ifndef ENGINE_FACE_H
#define ENGINE_FACE_H

#include <iostream>
#include <vector>

using namespace std;

class Face {
public:

    Face(){};

    vector<int> point_indexes;

    Face(vector<int> input){
        point_indexes = input;
    }
};


#endif //ENGINE_FACE_H
