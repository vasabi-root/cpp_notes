#ifndef __PH_AFFINITY_H__
#define __PH_AFFINITY_H__

#ifdef CMAKE
#include <iostream>
#include "tgaimage.h"
#include "line.h"
#include "matrix.h"
#include "config.h"
#include <vector>
#else
#include <iostream>
#include "misc/tgaimage.cpp"
#include "line.h"
#include "matrix.h"
#include <vector>
#endif

using std::cin;
using std::cout;
using std::endl;

/*
        010
        y
        |
        |
        |
    000 |---------------x 100
       / 
      /
     /
    z
    001
*/

void test_affinity() {
    TGAImage img(500, 500, TGAImage::RGB);
    Matrix<double> cube = {
        {0, 0, 0, 1}, // 0
        {1, 0, 0, 1}, // 1
        {0, 1, 0, 1}, // 2
        {0, 0, 1, 1}, // 3 
        {1, 1, 0, 1}, // 4
        {0, 1, 1, 1}, // 5
        {1, 0, 1, 1}, // 6
        {1, 1, 1, 1}, // 7
    }; 
    // 12 рёбер!!!
    std::vector<std::pair<size_t, size_t>> lines = {
        {0, 1}, {2, 4}, {5, 7}, {3, 6}, 
        {0, 3}, {2, 5}, {4, 7}, {1, 6},
        {0, 2}, {1, 4}, {6, 7}, {3, 5}, 
    };
    double pi = 3.1415;
    Matrix<double> sizer = {
        {100, 0, 0, 0}, 
        {0, 100, 0, 0}, 
        {0, 0, 100, 0}, 
        {0, 0, 0, 1},
    };

    Matrix<double> shifter = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {200, 200, 200, 1},
    };
    cube *= sizer * shifter;
    cout << cube << endl;

    for (double angle = 0; angle < 2*pi; angle += pi/60) {
        img.clear();
        auto rotater = Matrix<>::rotate3dX(angle) 
                     * Matrix<>::rotate3dY(angle)
                     * Matrix<>::rotate3dZ(angle);
        auto rotated = cube*(-shifter)*rotater*shifter;
        // cout << cube;
        // cout << rotated;
        for (auto [a, b] : lines ) { 
            line(rotated[a], rotated[b], ph::red, img); }
        img.write_tga_file("output.tga");
        Sleep(50); 

    }

    cout << cube << endl;
    cout << cube.transpose() << endl;

    // for ()








}






#endif

