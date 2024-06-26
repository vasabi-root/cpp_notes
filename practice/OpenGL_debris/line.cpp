#include <iostream>
#include "tgaimage.h"
#include "model.h"
#include "config.h"
#include <windows.h>
#include "../bench.cpp"

// #include <iostream>
// #include "misc/tgaimage.cpp"
// #include "config.h"
// #include <windows.h>
// #include "../bench.cpp"

// cmake -G "MinGW Makefiles" ..

const char* imgName = "output.tga";

void parameter_ptrs(int x0, int y0, int x1, int y1, TGAColor color, TGAImage &img) {
    float dt;
    int x = 0, y = 0;
    int *px=&x, *py=&y;

    if (std::abs(x1-x0) < std::abs(y1-y0)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        px = &y;
        py = &x;
    }
    dt = 1.0 / (std::abs(x1-x0)+1);

    for (float t = 0.0; t < 1.0; t+=dt) {
        x = t*(x1-x0) + x0;
        y = t*(y1-y0) + y0;
        img.set(*px, *py, color);
    }
}

void parameter_int_ptrs(int x0, int y0, int x1, int y1, TGAColor color, TGAImage &img) {
    float dt;
    int x = 0, y = 0;
    int *px = &x, *py = &y;

    if (std::abs(x1-x0) < std::abs(y1-y0)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        px = &y;
        py = &x;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int p0 = x1-x0;

    for (x = x0; x < x1; ++x) {
        float t = float(x-x0) / p0;
        y = t*(y1-y0) + y0;
        img.set(*px, *py, color);
    }
}

void parameter_ifs(int x0, int y0, int x1, int y1, TGAColor color, TGAImage &img) {
    float dt;
    bool steep = false;

    if (std::abs(x1-x0) < std::abs(y1-y0)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    dt = 1.0 / (std::abs(x1-x0)+1);

    for (float t = 0.0; t < 1.0; t+=dt) {
        int x = t*(x1-x0) + x0;
        int y = t*(y1-y0) + y0;
        
        if (steep) {        
            img.set(y, x, color);
        } else {
            img.set(x, y, color);
        }
    }
}

void parameter_int_ifs(int x0, int y0, int x1, int y1, TGAColor color, TGAImage &img) {
    float dt;
    bool steep = false;

    if (std::abs(x1-x0) < std::abs(y1-y0)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int p0 = x1-x0;

    for (int x = x0; x < x1; ++x) {
        float t = float(x-x0) / p0;
        int y = t*(y1-y0) + y0;
        if (steep) {
            img.set(y, x, color);
        } else {
            img.set(x, y, color);
        }
    }
}

void bresenham_ptrs(int x0, int y0, int x1, int y1, TGAColor color, TGAImage &img) {
    float dt;
    int x = 0, y = 0;
    int *px = &x, *py = &y;

    if (std::abs(x1-x0) < std::abs(y1-y0)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        px = &y;
        py = &x;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = 2*(x1-x0);
    int dy = 2*(y1-y0); 
    int y_inc = dy > 0 ? 1 : -1;
    dy = dy>0 ? dy : -dy;
    int error = dy - dx/2;

    for (x = x0, y = y0; x < x1; ++x) {
        img.set(*px, *py, color);
        if (error > 0) {
            y += y_inc;
            error -= dx;
        }
        error += dy;
    }
}

void bresenham_ifs(int x0, int y0, int x1, int y1, TGAColor color, TGAImage &img) {
    float dt;
    bool steep = false;

    if (std::abs(x1-x0) < std::abs(y1-y0)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = 2*(x1-x0);
    int dy = 2*(y1-y0); 
    int y_inc = dy > 0 ? 1 : -1;
    dy = dy>0 ? dy : -dy;
    int error = dy - dx/2;

    for (int x = x0, y = y0; x < x1; ++x) {
        if (steep) {
            img.set(y, x, color);
        } else {
            img.set(x, y, color);
        }

        if (error > 0) {
            y += y_inc;
            error -= dx;
        }
        error += dy;
    }
}

void wu(int x0, int y0, int x1, int y1, TGAColor color, TGAImage &img) {

}   

void draw_line(int x0, int y0, int x1, int y1, TGAColor color, TGAImage &img, bool smooth=true) {
    if (smooth)
        return wu(x0, y0, x1, y1, color, img);
    return bresenham_ptrs(x0, y0, x1, y1, color, img);
}

using drawing_func = void (int, int, int, int, TGAColor, TGAImage &);

void play_line(drawing_func* func) {
    int width = 500, height = 500;
    auto img = TGAImage(width, height, TGAImage::RGB);
    int x0 = width/2, y0 = height/2, r = width/4;
    int x1, y1;
    double pi = 3.1415;

    for (auto t = 0.0; t < pi*1.99; t += pi/50) {
        // img.clear();
        x1 = x0 + r*std::cos(t);
        y1 = y0 + r*std::sin(t);
        func(x0, y0, x1, y1, tga::red, img);
        img.write_tga_file(imgName);
        Sleep(50);
    }
}

void test_drawing_func(drawing_func* func, TGAImage &img, int iterNum) {
    for (auto i = 0; i < iterNum; ++i) {
        func(100, 25, 75, 100, tga::red, img);
        func(75, 100, 100, 175, tga::red, img);
        func(100, 175, 125, 100, tga::red, img);
        func(125, 100, 100, 25, tga::red, img);
    }
}

void test_model_obj(drawing_func* func, const char* obj_file_name) {
    auto model = std::unique_ptr<Model>(new Model(obj_file_name));
    TGAImage img(1500, 1500, TGAImage::RGB);
    int width = img.get_width();
    int height = img.get_height();

    for (int i=0; i<model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        for (int j=0; j<3; j++) {
            Vec3f v0 = model->vert(face[j]);
            Vec3f v1 = model->vert(face[(j+1)%3]);
            int x0 = (v0.x+1.)*width/2.;
            int y0 = (v0.y+1.)*height/2.;
            int x1 = (v1.x+1.)*width/2.;
            int y1 = (v1.y+1.)*height/2.;
            func(x0, y0, x1, y1, tga::white, img);
        }
    }
    img.flip_vertically();
    img.write_tga_file(imgName);
}

void test_draw_line() {
    auto img = TGAImage(200, 200, TGAImage::RGB);
    int iterNum = 1000000;
    Bench bench;
    
    bench.start();
    test_drawing_func(&parameter_ifs, img, iterNum);
    cout << " parameter_ifs:      " << iterNum/bench.measure() << endl;

    bench.start();
    test_drawing_func(&parameter_ptrs, img, iterNum);
    cout << " parameter_ptrs:     " << iterNum/bench.measure() << endl;

    bench.start();
    test_drawing_func(&parameter_int_ifs, img, iterNum);
    cout << " parameter_int_ifs:  " << iterNum/bench.measure() << endl;

    bench.start();
    test_drawing_func(&parameter_int_ptrs, img, iterNum);
    cout << " parameter_int_ptrs: " << iterNum/bench.measure() << endl;

    bench.start();
    test_drawing_func(&bresenham_ifs, img, iterNum);
    cout << " bresenham_ifs:      " << iterNum/bench.measure() << endl;

    bench.start();
    test_drawing_func(&bresenham_ptrs, img, iterNum);
    cout << " bresenham_ptrs:     " << iterNum/bench.measure() << endl;

    img.write_tga_file(imgName);
}

int main(int argc, char **argv) {
    play_line(&bresenham_ptrs);
    // test_draw_line();
    // test_model_obj(&bresenham_ptrs, "../obj/african_head.obj");
    return 0;
}