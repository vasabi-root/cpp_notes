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
    int priv = 0;
protected:
    int prot = 1;
public:
    int pub = 2;

    Base(int priv, int prot, int pub) : priv(priv), prot(prot), pub(pub) {}
    Base(int same) : priv(same), prot(same), pub(same) {
        cout << "Base(same)\n";
    }
    Base() : priv(0), prot(1), pub(2) {
        cout << "Base()\n";
    }
    Base(const Base& base) {
        priv = base.priv;
        prot = base.prot;
        pub  = base.pub;
        cout << "Base(Copy)\n";
    }
};

class Derived: public Base {
public:
    int d_pub;

    Derived(): Base(5), d_pub(1) {};
};

class DerivedPrivate: private Base {
public:
    int d_pub;

    DerivedPrivate(): Base(5), d_pub(1) {};
};

void f(Base b) {
    cout << "F";
}




struct Father {
    int f = 1;
};

struct Mother {
    int m = 2;
};

struct Son: public Father, public Mother {
    int s = 3;
};



int main() {
    while(0)
    {
        Derived d;
        Base b1 = static_cast<Base>(d); // тождественно
        Base b2 = d;                    // этому

        Base& rb1 = static_cast<Base&>(d); // тождественно
        Base& rb2 = d;                     // тождественно

        // Derived rd = rb2; // нельзя неявно кастовать к наследнику!
        // Но можно кастовать ЯВНО ссылки! Работает, только если до этого Base 
        // был проинициализирован Derived, иначе -- UB
        Derived& rd = static_cast<Derived&>(rb2); 

        d.d_pub = 30;
        cout << rd.d_pub << endl;


        DerivedPrivate d_priv;
        // Base b3 = d_priv; // нельзя получить доступ к родителю извне private-наследника
        f(d);
    }

    {
        Son s;
        Father &f = s;
        Mother &m = s;

        cout << &s << endl;
        cout << &f << endl;
        cout << &m << endl << endl; // другой адрес (+4Б)

        cout << &s << endl;                    // same
        cout << static_cast<Son*>(&f) << endl; // same
        cout << static_cast<Son*>(&m) << endl; // same -- каст сдвинул на 4Б вперёд!
        cout << endl;

        Mother m1;

        cout << static_cast<Son*>(&m1) << endl; // каст сдвинул на 4Б назад
        cout << reinterpret_cast<Son*>(&m1) << endl; // UB, но адрес тот
        cout << reinterpret_cast<Son&>(m1).f << endl; // UB, но мы кастонули к сыну
        //            [M]
        // у которого [F] [M] [S] => expr.f == m1
        
        cout << &m1 << endl;
        // cout << &f1 << endl;
        cout << *((int*)(&m1) + 1) << endl;

        cout << static_cast<Son&>(m1).s << endl;
    }

    return 0;
}