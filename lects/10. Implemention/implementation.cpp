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

class Base {
private:
    int priv = 10;
protected:
    int prot = 1;
public:
    int a = 0;
    void f() {
        cout << "Base F\n";
    }
};

class DerivedPrivate: private Base {
public:
    int b = 1;
    void g() {
        cout << "G\n";
        prot = 1;
        // priv = 1;
    }
    friend int main();
};

class DerivedProtected: protected Base {
public:
    int b = 1;
    void g() {
        cout << "G\n";
        // prot = 1;
    }
};

class Granny {
protected:
    int a = 0;
    void f(int a) {
        cout << "Granny\n";
    }
public:
    Granny(int a): a(a) {
        cout << "Granny\n";
    }
};

class Mommy: public Granny {
protected:
    // using Granny::f;
public:
    using Granny::f;
    void f(double) {        // при вызове m.f(0) будет вызвана эта функция с приведением к даблу
        cout << "Mommy\n";
    }

    using Granny::Granny;

    // Mommy(int a): Granny(a) {
    //     cout << "Mommy\n";
    // }

};

class Son: public Mommy { 

};

int main() {
    DerivedPrivate d;
    d.f(); // if main is friend for DerivedPrivate
    d.g();
    // d.p;

    DerivedProtected dprot;
    // dprot.f();

    Mommy m(1);
    m.f(0);

    Granny g = m;
    Granny& rg = m;
    Granny* pg = &m;
    // Mommy& rm = g; // CE 
    // Mommy &m = dynamic_cast<Mommy&>(g); // only for polymorphic classes

    return 0;
}