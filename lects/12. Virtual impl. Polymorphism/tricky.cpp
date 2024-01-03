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
    virtual void f() = 0;

    void g() {
        f();
    }
};

struct Derive: Base{
    void f() {
        cout << "d\n";
    }
};

int main() {
    {
        Derive d;
        d.g();
    }

    return 0;
}