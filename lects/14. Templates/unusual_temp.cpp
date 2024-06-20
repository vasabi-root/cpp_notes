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

template <> // специализация TU
void f(int a, int b) { // никогда не вызовется, если есть 
                       // перегрузка для TT
    cout << " 2\n";
}

// перегрузка
template <typename T = double> // работает и без double по умолчанию
void f(T a, T b=1.5) {
    cout << "3\n";
}

template <> // специализация TT
void f(int a, int b) { // b = int(1.5) по умолчанию!
    cout << "4, b = " <<  b << endl;
}

void f(int a, int b) { // перегрузка
    cout << "5\n";
}



template <int N>
struct S{
    bool b = N ? 1 : 0;
};


template <typename T, typename U = int>
struct Struct{ };

template <typename T>
using myvec = std::vector<T>;

// тут Container -- шаблонный тип, который принимает один шаблонный параметр (анонимный)
template <typename T, template <typename> typename Container = myvec>
struct  Stack {
    Container<T> c;
};

// template <typename T, template <typename, typename> typename Container = Struct>
// struct  Stack {
//     Container<T> c;
// };

int main() {
    // while(0)
    {
        f(1, 1); // 5
        f(1); // 4
        // f<int, char>(1.0); // CE
    }
    {
        S<1> s1;
        S<2> s2;
        cout << sizeof(S<1>) << endl; // 1...
    }
    {
        int x = 0;
        // constexpr int y = x; // CE
        std::vector<int> v(1);
        Stack<int> s;
    }
    {

    }
    return 0;
}