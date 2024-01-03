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

struct Father {
    int a = 1;
};

struct Mother {
    int a = 2;
};

struct Son: public Father, public Mother {
    int a = 3;
};



int main() {
    Son s;
    cout << s.Father::a << '\n';
    cout << s.Mother::a << '\n';
    cout << s.a << '\n';

    return 0;
}