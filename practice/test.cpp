#include <iostream>
#include <vector>
#include <windows.h>

using std::cin;
using std::cout;
using std::endl;


template <typename T=int, size_t N=0>
struct S {

    S(std::initializer_list<std::initializer_list<T>> list) {
        cout << "lst" << endl;
        // N = 9; // CE
    }
    
    S(std::initializer_list<T> list) {
        cout << "int" << endl;
    }
};


int main() {
    cout << endl;
    Sleep(1000);
    S s1 = { 0, 0, 0, 0};
    S s2 = {{0, 0}, {0, 0}};

    return 9;
}