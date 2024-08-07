// #define CMAKE

#ifdef CMAKE
#include <iostream>
#include "tgaimage.h"
#include "model.h"
#include "config.h"
#include <windows.h>
#include "../bench.h"

#else
#include <iostream>
#include "misc/tgaimage.cpp"
#include "model.cpp"
#include "config.h"
#include <windows.h>
#include "../bench.h"
#endif

#include "line.h"
#include "matrix.h"
#include "affinity.h"
#include "vec.h"

void test_line() {
    play_line(&bresenham_ptrs);
    // test_draw_line();
    // test_model_obj(&bresenham_ptrs, "../obj/african_head.obj");
}

void test_matrix() {
    // test_plus();
    // test_brackets();
    test_multiply();
    // load_multiply_test();
}

int main(int argc, char **argv) {
    cout << endl; 
    Sleep(1000);
    // test_append();
    // play_line(&bresenham_ptrs);
    // test_affinity();
    // test_multiply();
    // test_vec();
    // test_vec4();
    // test_model_obj(&bresenham_ptrs, "obj/african_head.obj");
    test_model();

    
    
    return 0;
}