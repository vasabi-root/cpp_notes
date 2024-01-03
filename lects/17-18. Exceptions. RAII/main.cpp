#include <iostream>
#include <vector>

using std::cout;
using std::cin;
using std::endl;

using std::vector;

struct S {
    int p;
    S(const int &p): p(p) {}
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


int main() {
    ff();
    
    // S s(0), ss(1);
    cout << "end of a programm" << endl;
    return 0;
}
 