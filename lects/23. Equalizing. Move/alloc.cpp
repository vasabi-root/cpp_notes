#include <iostream>
#include <vector>

using std::cout;
using std::cin;
using std::endl;

template <typename T>
struct Alloc {
    T* allocate(size_t n) {
        return reinterpret_cast<T*>(new char[n*sizeof(T)]);
    }
    void deallocate(T* p, size_t) { // тут должно быть число, равное тому, 
                                    // с которым делался allocate на этот указатель
        delete [] reinterpret_cast<char*>(p);
    }

    template <typename... Args>
    void construct(T* p, Args& ...args) {
        new(p) T(args...) ;
    }

    void destroy(T* p) {
        p->~T();
    }
};

void* operator new(size_t n) {
    void* ptr = std::malloc(n);
    cout << "my new" << endl;
    return ptr;
}

int main() {
    int a = 1;
    int b = 2;
    int x = std::move(b);
    cout << x << " " << b << endl;
    int *pa = new int(1);
    delete pa;
    return 0;
}