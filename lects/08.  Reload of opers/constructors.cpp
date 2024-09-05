#include <iostream>
#include <cstring>
#include <algorithm>

using std::cout;
using std::cin;
using std::endl;

struct A {};
struct B: A {
    int a;
    const int& b = 0;

    B(int a, const int &b) : A(), a(a), b(b) {} // переприсвоение константной ссылки
    B() : B(0, a) {}
    // B() : B(0, 0) {} //  UB !!!!!! ref on local zero!!!!
    // B() : B(0, a), a(0) {} // CE
};

int main() {
    B b1(1, 1);
    B b2;
    cout << b1.a << " " << b1.b << endl;
    cout << b2.a << " " << b2.b << endl;
    return 0;
}