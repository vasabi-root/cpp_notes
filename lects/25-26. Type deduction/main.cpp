#include <iostream>
#include <vector>
#include <unordered_map>

using std::cin;
using std::cout;
using std::endl;

template <typename Container>
decltype(auto) getByIdx(Container &cont, size_t idx) {
  return cont[idx];
}

int main() {
    int a = 0;
    decltype(a) b = 2; // CT
    // decltype(++a) c; // CE -- [++a] == int&, необходимо проинициализировать ссылку
    decltype((a)) c = b; // int&

    return 0;
}