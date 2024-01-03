#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::cin;
using std::endl;

using std::vector;

class C1 {
private:
    int a;
public:
    void f();           // объявление функции
};

class C2 {
private:
    int a;
    friend void C1::f();// объявление этой ф-и дружественной
};

void C1::f() {          // определение
    C2 c2;
    c2.a = 1;
}

class C3 { // singletone
private:
    static C3 *obj;
    C3() {}

public:
    static C3& getObject() {
        if (obj)
            return *obj;
        
        obj = new C3;
        return *obj;
    }
    ~C3() {
        delete obj;
    }
};

C3* C3::obj = nullptr;

struct UserId {
    int id = 0;

    UserId(int id): id(id) {}

    operator int() { // приведение к int
        return id;
    }
};




int main() {
    C1 c1;
    c1.f();
    return 0;
}