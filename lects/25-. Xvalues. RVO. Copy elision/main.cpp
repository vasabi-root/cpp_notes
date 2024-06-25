#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;


struct S {
    int p;
    S(int pp): p(pp) { cout << "s " << p << endl; }
    S(const S& s): p(s.p) { cout << "sc " << p << endl; }
    S(S&& s): p(std::move(s.p)) { cout << "msc " << p << endl; }

    S& operator += (const S& s) {
        p += s.p;
        return *this;
    }
};

// -------------------------------------------------------------
S operator + (const S& a, const S& b) { // named RVO
    S copy = a; // 1sc
    copy += b;
    return copy; 
    // если сделать return copy += b;
    // то RVO не сработает и будет 2sc (даже не мув)!!!
}

// S operator + (S a, const S& b) {  // move-constructor
//     a += b;    
//     return a; // 1msc
// }

// S operator + (S a, const S& b) { // RVO !!!
//     return a += b; // 1sc
// }

// -------------------------------------------------------------
S g(int n) {
    // S s = S(n) += 1; // 2s 1sc
    // return s;
    // return S(n);
    return S(n) + 1;    // 2s 1sc
    // return S(n) += 1;    // 2s 1sc
}

template<typename T>
std::vector<T> f(T n) {
    return std::vector<T>(n);
}



int main() {
    cout << endl << endl;
    // сколько раз вызовется конструктор?
    std::vector<int> v = std::vector<int>(10, 1); 
    // раньше мы бы сказали, что 2 раза: для `std::vector<int>(10, 1)` и копии для `v`
    // теперь мы знаем мув-семантику и понимаем, что вызовется мув-конструктор копии
    // но в реале даже этого не происходит
    // компилятор умеет (а с c++17, должен) такое оптимизировать
    // это и называется copy elision

    std::vector<int> vint = f(10);
    // и даже здесь, когда возвращается, по идее, копия, вызовется всего один конструктор!!!
    // парадокс в том, что если бы мы возвращали ссылку на временный объект (что, в общем-то, запрещено) -- было бы 2 вызова

    cout << endl;
    S s = S(S(S(S(S(S(2)))))); // 1s

    cout << endl;
    S sg = g(10); // 3
    g(10);        // 3


    cout << endl;
    int x = g(10).p; // xvalue => TM => 1s 1sc

    cout << endl;
    S sx = S(S(S(S(S(S(2).p).p).p).p).p).p; // 7s

    cout << endl;
    S ss = S(1) + S(1);
    cout << endl;
    S sss = 1+1;
    return 0;
}