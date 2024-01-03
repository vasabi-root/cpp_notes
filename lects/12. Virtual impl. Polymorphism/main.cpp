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

struct Granny {
    int g = 0;
};

struct Father: public virtual Granny {
    int f = 1;
};

struct Mother: public virtual Granny {
    int m = 2;
};

struct Son: public Father, private virtual Granny { // public Mother
    int s = 3;
};



int main() {
    {
        // Son s;
        // // s.g; // CE без virtual
        // // Granny g = s; // CE без virtual
        // cout << &s << endl << &s.f << endl << &s.m << endl << &s.s << endl << &s.g << endl;
        // cout << sizeof(Father) << endl;
        // Father &f = s;
        // cout << sizeof(f) << endl;
        // cout << sizeof(Mother) << endl;
        // Mother &m = s;
        // cout << sizeof(m) << endl;
        // cout << sizeof(Son) << endl;
    }
    {
        Son s;
        s.g;
        cout << sizeof(s) << endl;
    }

    return 0;
}