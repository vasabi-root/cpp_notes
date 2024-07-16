#include <vector>
#include <iostream>

using std::vector;
using std::cin;
using std::cout;
using std::endl;

struct S {
    S() { cout << "S" << endl; }
    S(const S&) { cout << "S&" << endl; }
    S(S&&) noexcept { cout << "S&&" << endl; }
    ~S() { cout << "~" << endl; }
};

int main() {
    vector<S> v;  // capacity 1
    v.reserve(2); // capacity 2


    // v.push_back(S()); // S S& ~ 
    // v.push_back(S()); // S S& ~
    // v.push_back(S()); // S S&S& S& ~~~

    // uncomment move-constructor
    // v.push_back(S()); // S S&& ~ 
    // v.push_back(S()); // S S&& ~
    // v.push_back(S()); // S S&&S&& S&& ~~~

    v.emplace_back(); // S 
    v.emplace_back(); // S
    v.emplace_back(); // S S&& S&& ~~
    
    return 0; // ~~~
}