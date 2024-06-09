#include <iostream>
#include <vector>
#include "../bench.cpp"

using std::cin;
using std::cout;
using std::endl;

bool TEST = false;

// --------- классическая RVO реализация + ----------------------------------------
struct RvoRef {
    int x;
    RvoRef(int xx): x(xx) { if (TEST) cout << "s " << x << endl; }
    RvoRef(const RvoRef& s): x(s.x) { if (TEST) cout << "sc " << x << endl; }
    RvoRef(RvoRef&& s): x(std::move(s.x)) { if (TEST) cout << "msc " << x << endl; }

    RvoRef& operator += (const RvoRef& s) { x += s.x; return *this; }
};

RvoRef operator + (const RvoRef& a, const RvoRef& b) { // named RVO
    RvoRef copy = a; // +1sc
    copy += b;
    return copy; 
    // если сделать return copy += b;
    // то RVO не сработает и будет 2sc (даже не мув)!!!
}

// --------- плохая RVO реализация + ----------------------------------------
struct BadRvoRef {
    int x;
    BadRvoRef(int xx): x(xx) { if (TEST) cout << "s " << x << endl; }
    BadRvoRef(const BadRvoRef& s): x(s.x) { if (TEST) cout << "sc " << x << endl; }
    BadRvoRef(BadRvoRef&& s): x(std::move(s.x)) { if (TEST) cout << "msc " << x << endl; }

    BadRvoRef& operator += (const BadRvoRef& s) { x += s.x; return *this; }
};

BadRvoRef operator + (const BadRvoRef& a, const BadRvoRef& b) { // named RVO
    BadRvoRef copy = a; // +1sc
    return copy += b;  //  +1sc
    // RVO не сработает и будет 2sc (даже не мув)!!!
}

// --------- move-реализация + по ссылке ----------------------------------------
struct MoveRef {
    int x;
    MoveRef(int xx): x(xx) { if (TEST) cout << "s " << x << endl; }
    MoveRef(const MoveRef& s): x(s.x) { if (TEST) cout << "sc " << x << endl; }
    MoveRef(MoveRef&& s): x(std::move(s.x)) { if (TEST) cout << "msc " << x << endl; }

    MoveRef& operator += (const MoveRef& s) { x += s.x; return *this; }
};

MoveRef operator + (const MoveRef& a, const MoveRef& b) { // named RVO
    MoveRef copy = a; // +1sc
    copy += b;
    return std::move(copy); // +1 msc
}

// --------- move-реализация + по значению ----------------------------------------
struct MoveVal {
    int x;
    MoveVal(int xx): x(xx) { if (TEST) cout << "s " << x << endl; }
    MoveVal(const MoveVal& s): x(s.x) { if (TEST) cout << "sc " << x << endl; }
    MoveVal(MoveVal&& s): x(std::move(s.x)) { if (TEST) cout << "msc " << x << endl; }

    MoveVal& operator += (const MoveVal& s) { x += s.x; return *this; }
};

MoveVal operator + (MoveVal a, const MoveVal& b) { // move
    a += b;
    return a;
}

// --------- RVO-реализация + по значению ----------------------------------------
struct RvoVal {
    int x;
    RvoVal(int xx): x(xx) { if (TEST) cout << "s " << x << endl; }
    RvoVal(const RvoVal& s): x(s.x) { if (TEST) cout << "sc " << x << endl; }
    RvoVal(RvoVal&& s): x(std::move(s.x)) { if (TEST) cout << "msc " << x << endl; }

    RvoVal& operator += (const RvoVal& s) { x += s.x; return *this; }
};

RvoVal operator + (RvoVal a, const RvoVal& b) { // RVO
    return a += b;
}


template <typename T>
void test_sum(size_t n=300'000'000) {
    Bench bench;
    cout << endl;
    bench.start();
    for (size_t i=0; i < n; ++i) {
        T a = T(1) + T(2);
    }
    cout << typeid(T).name() << ": " << double(n) / bench.measure() << endl;
}

int main() {
    test_sum<RvoRef>();
    test_sum<BadRvoRef>();
    test_sum<MoveRef>();
    test_sum<MoveVal>();
    test_sum<RvoVal>();

    return 0;
}