#ifndef __PH_AFFINITY_H__
#define __PH_AFFINITY_H__

#ifdef CMAKE
#include <iostream>
#include "tgaimage.h"
#include "line.h"
#include "matrix.h"
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
    Matrix cube = {
        {0, 0, 0, 1}, // 0
        {1, 0, 0, 1}, // 1
        {0, 1, 0, 1}, // 2
        {0, 0, 1, 1}, // 3 
        {1, 1, 0, 1}, // 4
        {0, 1, 1, 1}, // 5
        {1, 0, 1, 1}, // 6
        {1, 1, 1, 1}, // 7
    };
    std::vector<std::pair<size_t, size_t>> lines = {
        {0, 1}, {2, 4}, {5, 7}, {3, 6},
        {0, 3}, {2, 5}, {4, 7}, {1, 6}
    };

    cout << cube << endl;
    cout << cube.transpose() << endl;

    // for ()

    auto rotate_mat = Matrix<>::rotate3dZ(300/3.14);







}






#endif

