#include <iostream>
#include <cmath>
#include <typeinfo>
#include <chrono>

using std::cin;
using std::cout;
using std::endl;


template <typename T>
struct S {
private:
    T a[10];
    size_t i = 0;

public:
    template <typename Head, typename ...Tail>
    S(Head h, Tail... t) {
        init(h, t...);
    }

    void init() {}
    template <typename Head, typename ...Tail>
    void init(Head h, Tail... t) {
        a[i++] = h; 
        init(t...);
    }

    friend std::ostream& operator << (std::ostream &out, const S &s) {
        out << " [ ";
        for (auto el : s.a) {
            out << el << " ";
        }
        out << "]" << endl;
        return out;
    }
};


int main() {
    S<int> s = {1, 2, 3, 4};
    cout << s;
    return 0;
}