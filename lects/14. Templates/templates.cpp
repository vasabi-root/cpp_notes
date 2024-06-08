#include <iostream>
#include <vector>

using std::cout;
using std::cin;
using std::endl;

using std::vector;

template <typename T>
class vec{

};

// Полная специализация
template <> 
class vec<bool> {

};

// Частичная специализация
template <typename T> 
class vec<T*> { // [], &, const, const &

};

template <typename T, typename U>
void f(T a, U b) {
    cout << "1\n";
}

template <>
void f(int a, int b) {
    cout << "2\n";
}

template <typename T = double>
void f(T a, T b=1.0) {
    cout << "3\n";
}

// void f(int a, int b) {
//     cout << "4\n";
// }



template <int N>
struct S{
    bool b = N ? 1 : 0;
};

// template <typename T, template<typename> class Container = std::vector>
// template <typename T, typename Container = std::vector<T>>
struct  Stack {
    Container<T> c;
};




int main() {
    while(0)
    {
        f(1, 1); // 3
        f(1); // 3
        // f<int, char>(1.0); // CE
    }
    {
        S<1> s1;
        S<2> s2;
        cout << sizeof(S<1>) << endl; // 1...
    }
    return 0;
}