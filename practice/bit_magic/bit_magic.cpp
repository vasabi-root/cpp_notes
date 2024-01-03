#include <iostream>
#include <vector>

using std::cout;
using std::cin;
using std::endl;

using std::vector;

uint8_t naive(const int &);
uint8_t minusOne(const int &);
uint8_t mod(const int &);

void printAll(const int &);

int main() {
    printAll(323'456);
    printAll(16);
    printAll(0);
}

void printAll(const int &a) {
    cout << endl;
    cout << "a = " << a << endl;
    cout << "naive:    " << (int)naive(a) << endl;
    cout << "minusOne: " << (int)minusOne(a) << endl;
    cout << "mod:      " << (int)mod(a) << endl;
    cout << endl;
}

uint8_t naive(const int &a) {
    uint8_t bits_num = 0;
    for (auto i = a; i; bits_num += i & 0x01, i >>= 1) {}
    return bits_num;
}

uint8_t minusOne(const int &a) {
    uint8_t bits_num = 0;
    for (auto i = a; i; i &= i-1, ++bits_num) {}
    return bits_num;
}

uint8_t mod(const int &a) {
    uint8_t bits_num = 0;
    // for (auto i = a; i; i &= i-1, ++bits_num) {}
    return bits_num;
}