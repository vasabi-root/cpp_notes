#include <iostream>
#include <vector>

using std::cout;
using std::cin;
using std::endl;

int x = 0;

int main() 
{
    // int x = 3;
    // cout << x << endl;
    // {
    //     int x = 1;
    //     cout << x << endl;
    //     cout << ::x << endl;
    // }

    std::vector<int> v = {};
    // std::vector<int> v(0);
    cout << "v.size()          == " << v.size() << endl;
    // [v.size()] == [uint] => [v.size() - 1] == [uint]
    cout << "v.size() - 1      == " << v.size() - 1 << endl;
    cout << "(int)v.size() - 1 == " << (int)(v.size() - 1) << endl;
    cout << "0 - 1             == " << 0 - 1 << endl;
}
