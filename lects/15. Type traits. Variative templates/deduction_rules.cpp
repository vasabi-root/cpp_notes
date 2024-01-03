#include <iostream>
#include <vector>

using std::cout;
using std::cin;
using std::endl;

using std::vector;

template <typename T>
struct check {
    check() = delete;
};

template <typename T>
void f(T x) {
    check<T> c;
}

template <typename T>
void g(T& x) {
    check<T> c;
}

int main() {
    while (0)
    {
        // f<int>();
        int x;
        int &rx = x;
        const int &crx = rx;

        const int y = 1;
        
        // f(rx); // int
        // f(x); // int
        // f(crx); // int
        // f(y); // int

        // g(y); // const int !!!
        // g(crx); // const int
        g<int&>(rx); // int&, но компилятор схлопнет T& == int& & == int&

    }
    return 0;
}
