#include <iostream>
#include <any>
#include <unordered_map>

using std::cin;
using std::cout;
using std::endl;


namespace my {
class any {
private:
    struct Base { 
        virtual ~Base() {} 
        virtual Base* getCopy() const {}
    };

    template <typename T>
    struct Derived: public Base { 
        T object;
        Derived(const T& object): object(object) {}
        Derived(T&& object): object(std::move(object)) {}
        Base* getCopy() const override {
            return new Derived(object);
        }
    };

    Base *ptr;

    template <typename T>
    friend T cast(const any& a);

    template <typename T>
    friend std::remove_reference_t<T> cast(const any& a);


public:
    any(): ptr(new Derived(0)) {}
    any(const any& other): ptr(other.ptr->getCopy()) {}

    template <typename T>
    any(const T& object): ptr(new Derived(object)) {}

    template <typename T>
    any& operator = (const T& object) {
        delete ptr;
        ptr = new Derived(object);
        return *this;
    }

    template <typename T>
    any& operator = (T&& object) {
        delete ptr;
        ptr = new Derived(std::move(object));
        return *this;
    }

    ~any() {
        delete ptr;
    }
};

template <typename T>
std::remove_reference_t<T> cast(const any& a) {
    auto *d = dynamic_cast<const any::Derived<std::remove_reference_t<T>>*>(a.ptr);
    if (!d) { throw std::bad_any_cast(); }
    return d->object;
}


}

struct Base {
    // int a = 0;
    virtual ~Base() {};
};
struct Derived: Base {
    int b = 0;
    // ~Derived() { Base::~Base(); }
};

int main() {
    {
        double d = 1.0;
        double* pd = &d;
        double*& rpd = pd;
        double*&& rrpd = std::move(pd);
    }
    {
        std::any a = 10;
        cout << sizeof(a) << endl;
        cout << std::any_cast<int&>(a) << endl;

        a = 1.5;
        cout << std::any_cast<double&>(a) << endl;

        a = "sdfsd";

        cout << std::any_cast<const char*&>(a) << endl;

        a = std::pair{1, 2};
        cout << std::any_cast<std::pair<int, int>&>(a).first << endl;

        cout << sizeof(a) << endl;
    }
    {
        my::any a = 8;

        a = 0.1;
        cout << my::cast<double>(a) << endl;

        // a = "sdfsdf";
        // cout << my::cast<const char*&>(a) << endl;
    }
    {
        Derived* d = new Derived();
        Base* b = d;
        cout << dynamic_cast<Derived*>(b)->b << endl;
        delete d;
    }
    {
        Derived d;
        Base& b = d;
        cout << dynamic_cast<Derived&>(b).b << endl;
    }
    {
        std::unordered_map<int, int> umap = {{1, 2}, {12, 3}};
        cout << sizeof(umap) << endl; 
    }


    return 0;
}