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
    virtual void f() {
        cout << "B\n";
    }
};

struct Derived: Base {
    void f() override {
        cout << "D\n";
    }
};

int main() {
    Base b;
    b.f(); // B

    Derived d;
    d.f(); // D

    Base bb = d;
    bb.f(); // B, ибо копия

    Base &rb = d;
    rb.f(); // B, ибо в плюсах приоритет каста функции у родителя (в джаве наоборот)
    // Чтобы это изменить, помечаем версию родителя virtual
    return 0;
}