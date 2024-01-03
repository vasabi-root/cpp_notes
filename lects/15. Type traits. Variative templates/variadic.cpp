#include <iostream>
#include <vector>
#include <type_traits>

using std::cout;
using std::cin;
using std::endl;

using std::vector;
using std::is_same_v;

void print() {
    cout << endl;
}

// g++ -f_template_depth=10000
// но есть ещё одно ограничение на кол-во аргументов
template <typename Head, typename... Tail>
void print(const Head& head, const Tail&... tail) {
    cout << head << " ";
    print(tail...);
}

bool isHomogeneous() {
    return true;
}

template <typename A>
bool isHomogeneous(const A&) {
    return true;
}

template <typename A, typename B, typename... Tail>
bool isHomogeneous(const A&, const B& b, Tail... tail) {
    if (!is_same_v<A, B>)
        return false;
    
    return isHomogeneous(b, tail...);
}


template <typename A, typename B, typename... Tail>
struct is_homogeneous {
    static const bool value = is_same_v<A, B> && is_homogeneous<B, Tail...>::value;
};
template <typename A, typename B, typename... Tail>
const bool is_homogeneous_v = is_homogeneous<A, B, Tail...>::value;

template <typename A, typename B>
struct is_homogeneous<A, B> {
    static const bool value = is_same_v<A, B>;
};

int main() {
    {
        print(1, 12, "name", 1.0); // генерится 5 разных функций print
        cout << isHomogeneous(1.0, 1.0, 1.0) << endl;
        cout << is_homogeneous_v<int, int, float, float>;
    }
    return 0;
}