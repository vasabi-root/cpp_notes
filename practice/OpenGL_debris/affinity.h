#ifndef __PH_AFFINITY_H__
#define __PH_AFFINITY_H__

#include <iostream>
#include "tgaimage.h"
#include "line.h"
#include "matrix.h"
#include <vector>

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
    std::vector<Vec3d_f<>> cube = {
        {0, 0, 0},
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1},
        {1, 1, 0},
        {0, 1, 1},
        {1, 0, 1},
        {1, 1, 1},
    };

    for (auto v : cube) {
        cout << v;
    }




}






#endif

