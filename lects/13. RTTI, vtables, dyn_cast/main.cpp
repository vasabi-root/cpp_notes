#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <vector>
#include <set>

using std::cout;
using std::cin;
using std::endl;

using std::vector;

struct Base {
    int *a = new int();

    virtual ~Base() {
        delete a;
        cout << "B\n";
    }
};

struct Derived: public Base {
    int *b = new int();

    ~Derived() {
        delete b;
        cout << "D\n";
    }
};


int main() {
    Base *b = new Derived();

    delete b; // деструктор Derived НЕ вызовится

    return 0;
}