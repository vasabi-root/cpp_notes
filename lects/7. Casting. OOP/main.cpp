#include <iostream>
#include <vector>

using std::cout;
using std::cin;
using std::endl;

using std::vector;

void cf(const int &a) {
    cout << "const" << endl;
}

void cf(int &a) {
    cout << "var" << endl;
}



class C {
private:
    int a;
    double b;

    void printSum(int p) {
        cout << a+p << endl;
    }

public:
    void printSum(double p) {
        cout << a+p << endl;
    }
};

int main() {
    {
        double d = 0.0;
        int64_t& a64 = reinterpret_cast<int64_t&>(d);

        for (int i=sizeof(d)*8 - 1; i >= 0; --i) {
            cout << (!!(a64 & (1 << i)) ? 1 : 0);
            cout << (!(i % 8) ? " " : "");
        }

        cout << endl;
        cout << d << endl;
        cout << a64 << endl;
    }
    {
        int a = 10;
        const int &ca1 = a;
        const int &ca2 = ca1;
        int& nca = const_cast<int&>(ca2);
        

        nca = 11;

        cout << a << endl;
        cout << ca1 << endl;
        cout << ca2 << endl;
        cout << nca << endl;
    }
    {
        C c;
        // c.printSum(0); // CE
        c.printSum(0.0); // OK

    }
}