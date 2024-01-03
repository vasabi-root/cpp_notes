#include <iostream>
#include "matrix.cpp"
#include <array>
#include <vector>

using std::cout;
using std::cin;
using std::endl;

using std::array;

int main() {
    while(0)
    {
        Matrix<3, 3> m(1.1);
        m[1][2] = 100;
        cout << m << "\n";
        Matrix<3, 3> mm = m;
        cout << mm;
    }
    while(0)
    {
        Matrix<3, 3> m(1);
        cout << (m + 1) << endl;
    }
    {
        Matrix<3, 3> m(1);
        cout << -m << endl;
        cout << m-m << endl;
        cout << (3 - m) << endl;
        cout << (m -= 3) << endl;
    }

    // Matrix<3, 3> eye = Matrix<3, 3>::eye();
    // cout << eye;
    
}