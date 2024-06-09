#include <iostream>

using std::cout;
using std::cin;
using std::endl;

template <typename T>
void check_val(T&& x) {
    if (std::is_lvalue_reference_v<T>) {
        cout << "lvlr\n";
    } else {
        cout << "rvlr\n";
    }
}

// template <typename T>
// T& forward(T& x) { // std::remove_reference_t<T>&
//     // cout << "T&\n";
//     return static_cast<T&>(x);
// }

template <typename T>
T&& forward(T&& x) { // std::remove_reference_t<T>&&
    return static_cast<T&&>(x);
}

template <typename T>
T&& forward_(std::remove_reference_t<T>& x) { 
    return static_cast<T&&>(x);
}

template <typename T>
T&& forward_(std::remove_reference_t<T>&& x) {
    return static_cast<T&&>(x);
}

void g() {}

template <typename Head, typename... Tail>
void g(Head& head, Tail&&... tail) {
    cout << "lvl head\n";
    g(tail...);
}

template <typename Head, typename... Tail>
void g(Head&& head, Tail&&... tail) {
    cout << "rvl head\n";
    g(tail...);
}


void f(int&& a) {}
void f(const int& a) {}

struct S {
    void f() & {
        cout << "lvr\n";
    }

    void f() && {
        cout << "rvr\n";
    }
};

int main() {
    {
        int x = 5;
        int& r = x; // OK
        // int&& rr = x; // CE: попытка инициализировать rval через lval
        int&& rr = std::move(x); // OK: x -- умер
        int&& a = 5; // OK 
        int& b = rr; // OK
        // int&& c = rr; // CE: выр-е rr здесь lval (rval выр-е -- это вызов функции)

        const int& clvr1 = r; // OK
        const int& clvr2 = rr; // OK
        const int& clvr3 = std::move(rr); // OK: но константность останется с нами
        // int&& rvr = std::move(clvr1);// CE: хотим забрать органы того, кто запретил вмешательство в организм (same для lvr2 lvr3)

        // int& rvr = std::move(r); // CE: lval = rval;
        // int &rvr = 5;
        const int&& crvr = std::move(rr); // OK: но бессмысленно

        f(rr); // вызовется версия с const int&!!!! Ибо тут играет роль, какой тип выражения (lv rv), а не тип данных (метка всё ещё быстро сгорает)
        f(std::move(rr)); // вот тут вызовется int&&
    }
    {
        S s;
        s.f();   // объект lvl
        S().f(); // объект rvl
    }

    {
        int x = 5;
        int y = 6;
        int z = 7;
        // g(std::move(x), std::move(y), std::move(z));
        
        // int &&rvfx = forward(std::move(x));
        // int &lvfx = forward(x);

        // std::forward<int&&>(std::move(x));
        cout << endl;
        check_val(forward_<int>(x+0));            // r
        check_val(forward_<int&>(x=0));           // l
        check_val(forward_<int&>(5));             // l
        check_val(forward_<int&&>(std::move(5))); // r
        check_val(forward_<int&>(x));             // l
        check_val(forward_<int&&>(std::move(x))); // r

        cout << endl;
        check_val(forward(x+0));          // r
        check_val(forward(x=0));          // l
        check_val(forward(5));            // l!
        check_val(forward(std::move(5))); // r
        check_val(forward(x));            // l
        check_val(forward(std::move(x))); // r
    }

    return 0;
}