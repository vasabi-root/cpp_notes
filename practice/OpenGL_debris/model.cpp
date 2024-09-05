#ifndef __MODEL__CPP
#define __MODEL__CPP

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"
#include "line.h"
#include "config.h"
#include <windows.h>

Model::Model(const char *filename) : verts_(), faces_() {
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec4f v;
            for (int i=0;i<3;i++) iss >> v.raw[i];
            verts_.push_back(v);
        } else if (!line.compare(0, 2, "f ")) {
            std::vector<int> f;
            int itrash, idx;
            iss >> trash;
            while (iss >> idx >> trash >> itrash >> trash >> itrash) {
                idx--; // in wavefront obj all indices start at 1, not zero
                f.push_back(idx);
            }
            faces_.push_back(f);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# "  << faces_.size() << std::endl;
}

Model::~Model() {
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::nfaces() {
    return (int)faces_.size();
}

std::vector<int> Model::face(int idx) {
    return faces_[idx];
}

Vec4f Model::vert(int i) {
    return verts_[i];
}

template <typename T, typename Alloc>
Model& Model::operator *= (const Matrix<T, Alloc> &mat) {
    verts_ = verts_ * mat;
    return *this;
}

template <typename T, typename Alloc>
Model Model::operator * (const Matrix<T, Alloc> &mat) {
    Model copy = *this;
    copy *= mat;
    return copy;
}

TGAImage& Model::draw_wavefront(TGAImage& img) {
    int width = img.get_width();
    int height = img.get_height();

    for (int i=0; i<nfaces(); i++) {
        std::vector<int> face = faces_[i];
        for (int j=0; j<3; j++) {
            Vec4f v0 = verts_[face[j]];
            Vec4f v1 = verts_[face[(j+1)%3]];
            int x0 = (v0.x+1.)*width/2.;
            int y0 = (v0.y+1.)*height/2.;
            int x1 = (v1.x+1.)*width/2.;
            int y1 = (v1.y+1.)*height/2.;
            bresenham_ptrs(x0, y0, x1, y1, ph::white, img);
        }
    }
    return img;
}

void test_model() {
    auto model = std::make_unique<Model>("obj/african_head.obj");
    TGAImage img(100, 100, TGAImage::RGB);
    double pi = 3.1415, angle = pi / 20;

    auto rotator = Matrix<>::rotate3dY(angle); 
    // Matrix<>::rotate3dX(angle) 
    //              * Matrix<>::rotate3dY(angle)
                 
    for (auto t = 0.0; t < pi*1.99; t += angle) {
        img.clear();
        *model *= rotator;
        model->draw_wavefront(img);

        img.flip_vertically();
        img.write_tga_file("output.tga");
        Sleep(100);
    }
}

#endif