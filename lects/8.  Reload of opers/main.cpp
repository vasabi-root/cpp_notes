#include <iostream>
#include <vector>

using std::cout;
using std::cin;
using std::endl;

using std::vector;

struct Complex {
    double re = 0.0;
    double im = 0.0;

    Complex(double re, double im=0.0): re(re), im(im) {}

    Complex(const Complex &z) {
        cout << "C";
    }

    Complex& operator +=(const Complex &z) {
        re += z.re;
        im += z.im;
        return *this;
    }

    void print() {
        cout << re << '+' << im << "i" << endl;
    }

    /*
    Complex operator +(const Complex &z) {
        Complex copy = *this;
        copy += z;
        return copy;
    } // односторонний
    */
};

Complex operator +(const Complex &z1, const Complex &z2) {
    Complex copy(z1);
    copy += z2;
    return copy;
} // односторонний 

int main() {
    Complex z1(1.0, 1.0);
    Complex z2(-1.0, 0.0);

    (z1+z2).print();
    (z1+1.0).print();
    (1.0+z1).print();

    Complex sum(z1+z2);
    
    
    return 0;
}