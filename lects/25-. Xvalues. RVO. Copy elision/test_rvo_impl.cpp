#include <iostream>
#include <vector>
#include "../bench.cpp"


using std::cin;
using std::cout;
using std::endl;

bool TEST = false;

struct Base {
    int x;
    int y = 0;
    int z = 0;
    Base(int xx): x(xx) { if (TEST) cout << "s " << x << endl; }
    Base(const Base& s): x(s.x), y(s.y), z(s.z) { if (TEST) cout << "sc " << x << endl; }
    Base(Base&& s): x(std::move(s.x)), y(std::move(s.y)), z(std::move(s.z)) { if (TEST) cout << "msc " << x << endl; }

    Base& operator += (const Base& s) { x += s.x; return *this; }
};

Base operator + (const Base& a, const Base& b) { // named RVO
    Base copy = a; // +1sc
    copy += b;
    return copy; 
    // если сделать return copy += b;
    // то RVO не сработает и будет 2sc (даже не мув)!!!
}

// --------- классическая RVO реализация + ----------------------------------------
struct RvoRef: Base { 
    RvoRef(const Base& s): Base::Base(s) {}
    RvoRef(Base&& s): Base::Base(std::move(s)) {}
};

RvoRef operator + (const RvoRef& a, const RvoRef& b) { // named RVO
    RvoRef copy = a; // +1sc
    copy += b;
    return copy; 
    // если сделать return copy += b;
    // то RVO не сработает и будет 2sc (даже не мув)!!!
}

// --------- плохая RVO реализация + ----------------------------------------
struct BadRvoRef: Base { 
    // для implicit кастов определяем конструкторы от Base
    BadRvoRef(const Base& s): Base::Base(s) {}
    BadRvoRef(Base&& s): Base::Base(std::move(s)) {}
};

BadRvoRef operator + (const BadRvoRef& a, const BadRvoRef& b) { // named RVO
    BadRvoRef copy = a; // +1sc
    return copy += b;  //  +1sc
    // RVO не сработает и будет 2sc (даже не мув)!!!
}

// --------- move-реализация + по ссылке ----------------------------------------
struct MoveRef: Base { 
    MoveRef(const Base& s): Base::Base(s) {}
    MoveRef(Base&& s): Base::Base(std::move(s)) {}
};

MoveRef operator + (const MoveRef& a, const MoveRef& b) { // named RVO
    MoveRef copy = a; // +1sc
    copy += b;
    return std::move(copy); // +1 msc
}

// --------- move-реализация + по значению ----------------------------------------
struct MoveVal: Base { 
    MoveVal(const Base& s): Base::Base(s) {}
    MoveVal(Base&& s): Base::Base(std::move(s)) {}
};

MoveVal operator + (MoveVal a, const MoveVal& b) { // named RVO
    a += b;
    return a;
}

// --------- RVO-реализация + по значению ----------------------------------------
struct RvoVal: Base { 
    RvoVal(const Base& s): Base::Base(s) {}
    RvoVal(Base&& s): Base::Base(std::move(s)) {}
};

RvoVal operator + (RvoVal a, const RvoVal& b) { // named RVO
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
    cout << typeid(T).name() << ": " << n / bench.measure() << endl;
}

int main() {
    test_sum<Base>();
    test_sum<RvoRef>();
    test_sum<BadRvoRef>();
    test_sum<MoveRef>();
    test_sum<MoveVal>();
    test_sum<RvoVal>();

    return 0;
}