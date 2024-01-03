#include <iostream>
#include <vector>

using std::cout;
using std::cin;
using std::endl;

using std::vector;

template <typename T, typename U>
void f(T a, U b) {
    cout << "1\n";
}

template <typename T>
void f(T a, T b) {
    cout << "2\n";
}

void f(int a, int b) {
    cout << "3\n";
}

void f(int a, float b) {
    cout << "4\n";
}

template <typename T>
void f(T &a) {
    cout << "5\n";
}

template <typename T>
void f(T a) {
    cout << "6\n";
}

int main() {
    f(1, 1); // 3
    f(1, 1.0); // 1
    int a = 0;
    // f(a); // CE
    f(1); // 6 -- ибо от rvalue можно брать только константные ссылки
    return 0;
}
