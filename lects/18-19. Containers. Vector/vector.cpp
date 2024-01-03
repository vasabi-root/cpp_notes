#include <iostream>
#include <vector>

using std::cout;
using std::cin;
using std::endl;

// using std::vector;

template <typename T>
class vector {
private:
    T *arr;
    size_t sz;
    size_t cap;

public:
    size_t size() const;
    void resize(const size_t &new_size);

    size_t capacity() const;
    // reserve не эквивалентно созданию вектора размером new_cap!
    void reserve(const size_t& new_cap); 

    void push_back(const T& value);
    void pop_back();

    struct iterator;
};

template <>
class vector<bool> {
private:
    uint8_t *arr;
    size_t sz;
    size_t cap;

    struct BitReference {
        uint8_t &byte;
        uint8_t bit;

        operator bool() const {
            return byte & (1 << bit);
        }

        BitReference& operator = (const BitReference &) = default;
        BitReference& operator = (const int &) {
            byte 
        }
    };

public:
    vector(size_t sz, bool fill_with=0, size_t cap=800): sz(sz), cap(cap) {
        arr = new uint8_t[cap/8 + !!cap%8];
    }

    BitReference operator [] (const size_t &i) {
        return {*(arr + i/8), i % 8};
    }


};



int main() {
    std::vector<bool> vb(5, false);
    vb[2] = true;
    cout << typeid(vb[0]).name() << endl;
    vb[2];

    vector<bool> mvb(8);
    mvb[1] = 1;

    return 0;
}

template <typename T> 
void vector<T>::reserve(const size_t& new_cap) {
    if (new_cap <= cap) return;

    T* new_arr = reinterpret_cast<T*>(new uint8_t[new_cap * sizeof(T)]); // TODO remove reinterp_cast
    
    for (size_t i = 0; i < sz; ++i) {
        try {
            new(new_arr + i) T(arr[i]); // TODO how to avoid copying?   
        } catch(...) {
            for (size_t j = 0; j < i; ++j) {
                (new_arr + i)->~T;
            }
            delete [] reinterpret_cast<uint8_t*>(new_arr);
            throw;
        }
    }
    
    for (size_t i = 0; i < sz; ++i) {
        (arr+i)->~T();
    }

    delete [] reinterpret_cast<uint8_t*>(arr);
    cap = new_cap;
    arr = new_arr;
}

template <typename T> 
void vector<T>::push_back(const T& value) {
    if (sz >= cap) {
        // realloc
        reserve(cap * 2);
    }
    // push mew element to back
    // arr[sz] = value; // WRONG! мы лишь зарезервироваои память, 
                        // но не распределили её под тип объекта
    new(arr + sz) T(value); // TODO ES
    ++sz;
}

template <typename T>
void vector<T>::pop_back() {
    --sz;
    (arr+sz)->~T();
}

template <typename T>
struct vector<T>::iterator {
private:
    T* ptr;
public:
    T& operator * () const {
        return *ptr; // поэтому нельзя обращаться к итератору после vector change!
        // итератор бы не инвалидировался, если бы мы хранили указатель на вектор
        // но тогда теряем скорость! (2 прыжка по указателю вместо одного)
    }
};