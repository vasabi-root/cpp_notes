#include <iostream>

using std::cin;
using std::cout;
using std::endl;

namespace memory {

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

    T* ptr() { return ptr_; }

    ~unique_ptr() { Deleter()(ptr_); }
};

template <typename T>
class enable_shared_from_this {
    shared_ptr<T> sptr__;

public:
    shared_ptr<T> shared_from_this() {
        return sptr__;
    }
};


template <typename T, typename Deleter=std::default_delete<T>>
class shared_ptr {
private:
    T* ptr_;
    size_t *count_;

    struct ControlBlock {
        T object;
        size_t count;
    };

    shared_ptr(ControlBlock *pcb): ptr_(&pcb->object), count_(&pcb->count) {}
public:
    shared_ptr(T* ptr): ptr_(ptr), count_(new size_t) {
        *count_ = 1;
    }
    shared_ptr(const shared_ptr& other) noexcept
        : ptr_(other.ptr_), count_(other.count_) {}

    shared_ptr(shared_ptr&& other) noexcept
        : ptr_(other.ptr_), count_(other.count_) {
            other.ptr_ = nullptr;
            other.count_ = nullptr;
        }
    ~shared_ptr() {
        if (!count_) return;
        --*count_;
        if (*count_ == 0) {
            delete count_;
            cout << *ptr_ << endl;
            Deleter()(ptr_);
        }
    }
};

template <typename T, typename ...Args>
unique_ptr<T> make_unique(Args&&... args) {
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
    // можно без forward_if_noexcept, ибо new не кидает исключений
    // точнее, если он кинет, то он обязуется освободить то, что навыделял
}

template <typename T, typename ...Args>
shared_ptr<T> make_shared(Args&&... args) {
    void* p = ::operator new(sizeof(T) + sizeof(size_t));
    new (p) T(std::forward<Args>(args)...);
    new (static_cast<T*>(p)+1) size_t();
    cout << sizeof(*static_cast<T*>(p)) << " " << *static_cast<T*>(p) << endl;
    cout << sizeof(*reinterpret_cast<size_t*>(static_cast<T*>(p)+1)) << " " << *reinterpret_cast<size_t*>(static_cast<T*>(p)+1) << endl;
    return shared_ptr<T>(new T(std::forward<Args>(args)...));
}

struct S: enable_shared_from_this<S> { 
    int a, b, c; 
    S(int a, int b, int c): enable_shared_from_this(), a(a), b(b), c(c) {

    }
};

}


int main() {
    // auto sptr1 = memory::make_shared<int>(5);
    // auto sptr2 = memory::make_shared<std::string>(10, 'A');
    cout << sizeof(std::enable_shared_from_this<memory::S>) << endl;
    cout << sizeof(std::shared_ptr<memory::S>) << endl;
}