#include <iostream>
#include <string>
#include "static.cpp"

using std::cout;
using std::cin;
using std::endl;


S1 exs1;
struct S2
{
    int vari;
    float varf = exs1.varf;
};


int main() {

    S2 s2;
    cout << s2.varf;
    return 0;
}