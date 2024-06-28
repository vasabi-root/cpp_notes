#ifndef __PH_BENCH_H__
#define __PH_BENCH_H__

#include <iostream>
#include <cmath>
#include <typeinfo>
#include <chrono>

using std::cin;
using std::cout;
using std::endl;

using std::chrono::duration_cast;
using std::chrono::microseconds;
using namespace std::chrono;

class Bench {
private:
    high_resolution_clock::time_point time_pt_;

public: 
    // Старт счётчика времени
    void start() {
        time_pt_ =  high_resolution_clock::now();
    }

    // Расчёт времени, прошедшего со старта
    double measure() {
        auto end = high_resolution_clock::now();
        // floating-point duration: no duration_cast needed
        duration<double, std::ratio<1, 1>> diff = end - time_pt_;
        return diff.count();
    }
};

#endif