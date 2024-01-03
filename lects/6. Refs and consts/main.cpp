#include <iostream>
#include <vector>

using std::cout;
using std::cin;
using std::vector;

void f(int&);
void f(int);  // CE

int& bad(int x) {
    return x; // висячая ссылка (dongling ref)
}

int& notBad(int& x) {
    return x;
}

struct Ref {
    int &r;
    // Ref (int &r) : r(r) {};
};

int main() {
    vector<int> v = {1, 2, 3, 4, 5};
    vector<int> vv = v;  // создать копию
    vector<int> &rv = v; // сщздать ссылку (новое название)

    v[0] = 0;

    cout << v[0] << ' ' << vv[0] << ' ' << rv[0] << '\n'; // 0 1 0

    {
        // no garbage collection 
        vector<int> &rv2 = rv;
    } 
 
    // Constants
    const int x = 0;
    const int *pc = &x;
    pc++; // ok
    // *pc = 1; // CE

    // int p* = &x; // CE

    int y = 0;
    int* const cp = &y;
    // cp++; // CE
    *cp = 0; // ok

    const int& rcy = y; // rcy -- ссылка на константу y (константкая ссылка)

    {
        // lifetime expansion -- объект проинициализирован ссылкой => будет уничтожен 
        // это единственный случай, когда объект уничтожается раньше ссылки
        const vector<int>& crv = {1, 2, 3, 4};
    }

    int k = 10;
    int *pk = &k;

    int &rk = *pk;

    rk = 11;

    cout << "k = " << k << '\n';
    cout << "*pk = " << *pk << '\n';
    cout << "rk = " << rk << '\n';

    cout << sizeof(Ref);

    return 0;
}