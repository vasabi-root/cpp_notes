#ifndef __PH_MODEL3D_H__
#define __PH_MODEL3D_H__

#ifdef CMAKE
#include "tgaimage.h"
#else
#include "misc/tgaimage.cpp"
#endif

#include <iostream>
#include <cmath>
#include <fstream>
#include <windows.h>
#include <cassert>
#include <vector>

#include "matrix.h"
#include "vec.h"
#include "../bench.h"

using std::cin;
using std::cout;
using std::endl;

class Model {
public:
    Model(const char *filename);
	~Model();
	int nverts();
	int nfaces();
	Vec4f vert(int i);
	std::vector<int> face(int idx);

	template <typename T, typename Alloc>
	Model& operator *= (const Matrix<T, Alloc> &mat);

	template <typename T, typename Alloc>
	Model operator * (const Matrix<T, Alloc> &mat);

	TGAImage& draw_wavefront(TGAImage& img);
private:
    std::vector<Vec4f> verts_;
    std::vector<std::vector<int>> faces_;
};


#endif