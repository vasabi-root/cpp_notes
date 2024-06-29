#ifndef __PH_MODEL3D_H__
#define __PH_MODEL3D_H__

#include <iostream>
#include <cmath>
#include <fstream>
#include <windows.h>
#include <cassert>
#include <vector>
#include "matrix.h"
#include "../bench.h"

using std::cin;
using std::cout;
using std::endl;

class Model3d {
public:
    Model3d(): vertices_(0, 4) {};

private:
    Matrix<double> vertices_;
    std::vector<std::pair<size_t, size_t>> edges_;
};






#endif