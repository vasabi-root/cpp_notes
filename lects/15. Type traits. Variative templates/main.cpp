#include <iostream>
#include <vector>

using std::cout;
using std::cin;
using std::endl;

using std::vector;

template <typename T>
struct S{
    using X = T;
};

template <>
struct S<int>{
    static int X;
};

int a = 0;

template <typename T>
void f() {
    typename S<T>::X * a;
    // T == int:    X * a;
    // T == double: double *a;
}


template <typename T>
struct SS {
    template <int N, int M>
    struct A {};
};

template <>
struct SS<int> {
    static int A;
};

template <typename T>
void g() {
    typename SS<T>::template A<1, 2> a;
    // T == int: A < 1, 2 > a;
    // T != int: A<1, 2> a;
}

int main() {
    while (0)
    {
        // f<int>();
        f<double>();
    }
    return 0;
}
