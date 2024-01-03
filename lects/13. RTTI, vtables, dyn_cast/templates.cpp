#include <iostream>
#include <vector>

using std::cout;
using std::cin;
using std::endl;

using std::vector;

template <typename T>
const T& maximum(const T &a, const T &b) {
    return a > b ? a : b;
}

template <typename T>
using eq_map = std::map<T, T>;

int main() {

    return 0;
}