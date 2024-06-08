#include <iostream>
#include <vector>

using std::cout;
using std::cin;
using std::endl;

using std::vector;

struct S {
    int p;
    S(const int &p=0): p(p) {}
    ~S() noexcept(false) {
        if (p == 0)
            throw std::runtime_error("p is zero");
    }
};


struct Exception
{ std::string error; };

int f(const int &a, const int &b) {
    S s(0);
    if (b == 0) throw Exception{"Error: division by zero!"}; // копирование со стека в астрал
    return a / b;
}

void ff() {
    try {
        f(1, 0);
    } catch (const Exception &e) { // копирование с астрала на стек (если не юзать ссылку)
        cout << e.error << endl;
    }
}

void test() {
    try {
        S s[2];// смерть пр раскрутке стека
        // throw int(); // не получится словить что-то
    } catch (...) {
        cout << "\n\ncatсhed!\n";
    }
}

struct CD {
    CD() = default;
};

struct CD1 {
    CD1() {}
};


int main() {
    // ff();
    
    // // S s(0), ss(1);
    // cout << "end of a programm" << endl;

    CD cd;
    CD1 cd1;

    test();
    return 0;
}
 