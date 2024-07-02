#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>

using std::cin;
using std::cout;
using std::endl;

template <typename T, typename Deleter=std::default_delete<T>>
class unique_ptr {
private:
    T* ptr_;
public:
    unique_ptr(T* ptr): ptr_(ptr) { }

    unique_ptr(const unique_ptr&) = delete;
    unique_ptr(unique_ptr&& uptr) noexcept : ptr_(uptr.ptr_) {
        uptr.ptr_ = nullptr;
    }
    ~unique_ptr() { Deleter()(ptr_); }

    T* ptr() { return ptr_; }

    T& operator * () { return *ptr_; }
    T* operator -> () { return ptr_; }

};

struct S {
    int a;
};

void f(int* p) {
    p = nullptr;
}

int main() { 
    unique_ptr uptr1(new int(1));
    unique_ptr uptr2(std::move(uptr1));
    int *ptr = nullptr;

    int a = 1;
    int *p = &a;
    cout << p << endl;
    f(p);
    cout << p << endl;
    delete ptr;


    unique_ptr uptrs(new S);
}