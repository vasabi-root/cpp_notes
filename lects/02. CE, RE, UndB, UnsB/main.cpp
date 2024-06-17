#include <iostream>

using std::cout;
using std::cin;

int main() {
    float a = 1 / 0.0;
    int x = 0;
    ++x = x++;
    // ++x = ++x;

    cout << a << '\n';
    cout << x << '\n';
    return 0;
}
