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
    int b;


    virtual void f() {
        cout << "B\n";
    }

    virtual ~Base() {}
};

struct Derived: Base {
    int d; 

    void f() override {
        cout << "D\n";
    }

    ~Derived() {}
};



struct Granny {
    virtual void func() {
        cout << "1\n";
    }

    virtual ~Granny() {}
};

struct Father: public virtual Granny {
    int f = 1;
};

struct Mother: public virtual Granny {
    int m = 2;
};

struct Son: public Father, public Mother {
    int s = 3;
};

struct S{
    void f() {

    }
    void f1() {

    }
    void f2() {

    }
};


int main() {
    while(0)
    {
        int x;
        cin >> x;
        Base b;
        Derived d;
        Base &rb = x ? b : d; // RTTI

        rb.f(); // RTTI
    }

    {
        Mother *pm = new Son();
        Mother m = Mother();


        cout << dynamic_cast<Father*>(pm); // успех, ибо под pm лежит Son
        // cout << static_cast<Father*>(pm); // CE -- разные ветки
        // cout << dynamic_cast<Father*>(pm); // UB -- разные ветки, будет папа, под которым по факту лежит мама
        try {
            dynamic_cast<Father&>(m);
        } catch(...) {
            cout << "\nOOOps\n";
        }

        delete pm;

    }
    {
        cout << sizeof(S);
    }

    return 0;
}
