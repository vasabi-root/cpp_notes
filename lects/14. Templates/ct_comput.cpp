#include <iostream>

using std::cout;

template <size_t N>
struct Fibonacci {
    static const size_t value = Fibonacci<N-1>::value + Fibonacci<N-2>::value;
}; // шаблонная рекурсия по умолчанию -ftemplate-depth=1024 
// если разрешить слишком большую глубину рекурсии, то будет Seg fault в CT!!!!


template <>
struct Fibonacci<0> {
    static const size_t value = 0;
};

template <>
struct Fibonacci<1> {
    static const size_t value = 1;
};

int main() {
    cout << Fibonacci<20>::value; // O(N)
    return 0;
}