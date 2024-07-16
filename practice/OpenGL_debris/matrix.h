#ifndef __PH_MATRIX_H__
#define __PH_MATRIX_H__

#include <iostream>
#include <cmath>
#include <fstream>
#include <windows.h>
#include "../bench.h"
#include <cassert>

#ifdef CMAKE
#include "geometry.h"
#else
#include "misc/geometry.h"
#endif


using std::cin;
using std::cout;
using std::endl;

template <typename T, typename U> 
concept Assignable = requires (T t, U u)  { t = u; };

template <typename T, typename U> 
concept Plusable = requires (T t, U u)  { t += u; };

template <typename T, typename U> 
concept Multiplyable = requires (T t, U u)  { t *= u; };

template <typename T>
void is_initializer_list_f(std::initializer_list<T>);
template <typename T>
std::false_type is_initializer_list_f(T);

template <typename T>
struct is_initializer_list: decltype(is_initializer_list_f(std::declval<T>())) {};


template <template <typename...> class C, typename...Ts>
std::true_type is_base_of_template_impl(const C<Ts...>*);

template <template <typename...> class C>
std::false_type is_base_of_template_impl(...);

template <typename T, template <typename...> class C>
using is_base_of_template = decltype(is_base_of_template_impl<C>(std::declval<T*>()));

// T requires to have T(), T(1)
// TODO: add append() method (get rid of template M_ N_)
//       toScreen() method 

template <
    typename T = double,  
    typename Alloc_ = std::allocator<T>
>
requires (std::__is_allocator<Alloc_>::value)
class Matrix {
public:
    using Alloc = std::allocator_traits<Alloc_>; //::rebind_traits<T*>

    template <typename U>
    using RebindAlloc = typename std::allocator_traits<Alloc_>::template rebind_alloc<U>;

    template <typename U, typename UAlloc_> 
    requires (std::__is_allocator<UAlloc_>::value)
    friend class Matrix;

    template <typename... Args>
    Matrix(size_t M, size_t N, const Args&... args): 
    cells_(Alloc::allocate(alloc_, 2*M*N)), 
    M_(M), N_(N), size_(M*N), capacity_(2*size_)  {
        for (size_t i = 0; i < size_; ++i) {
            Alloc::construct(alloc_, cells_+i, args...);
        }
    }

    // template <bool U = true>
    Matrix (std::initializer_list<std::initializer_list<T>> list): 
    cells_( Alloc::allocate(alloc_, 2*list.size()*list.begin()->size())), 
    M_(list.size()), N_(list.begin()->size()), size_(M_*N_), capacity_(2*size_)
    {
        for (size_t i = 0; i < M_; ++i) {
            auto inner = *(list.begin()+i);
            for (size_t j = 0; j < N_; ++j) {
                Alloc::construct(alloc_, cells_+i*N_+j, 1);
                cells_[i*N_ + j] = *(inner.begin()+j);
            }
        }
    }

    // template <bool U = is_initializer_list<T>::value>
    Matrix (std::initializer_list<T> list): 
    cells_(Alloc::allocate(alloc_, 2*list.size())), 
    M_(1), N_(list.size()), size_(M_*N_), capacity_(2*size_) {
        for (size_t i = 0; i < size_; ++i) {
            Alloc::construct(alloc_, cells_+i, 1);
            cells_[i] = *(list.begin()+i);
        }
    }

    Matrix(const Matrix& other):
    cells_(Alloc::allocate(alloc_, other.capacity_)), 
    M_(other.M_), N_(other.N_), size_(other.M_*other.N_), capacity_(other.capacity_)  {
        for (size_t i = 0; i < size_; ++i) {
            Alloc::construct(alloc_, cells_+i, other.cells_[i]);
        }
    }

    Matrix(Matrix&& other) noexcept: 
    M_(other.M_), N_(other.N_), size_(other.M_*other.N_), capacity_(other.capacity_)  { //: cells_(Alloc::allocate(alloc_, M_*N_))
        std::swap(cells_, other.cells_);
        other.cells_ = nullptr;
    }

    void destroy_cells() {
        for (size_t i = 0; i < size_; ++i) { 
            Alloc::destroy(alloc_, cells_+i);
        }
        Alloc::deallocate(alloc_, cells_, size_);
    }

    ~Matrix() { 
        if (cells_) {
            destroy_cells();
        }
    }

    Matrix& operator = (const Matrix &other) noexcept {
        Matrix copy(other);
        M_ = copy.M_;
        N_ = copy.N_;
        size_ = copy.size_;
        capacity_ = copy.capacity_;
        std::swap(cells_, copy.cells_);
        copy.cells_ = nullptr;
        return *this;
    }

    Matrix& operator = (Matrix&& other) noexcept { 
        M_ = other.M_;
        N_ = other.N_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        std::swap(cells_, other.cells_);
        other.cells_ = nullptr;
        return *this;
    }

    template <typename U> requires Assignable<T, U>
    Matrix& operator = (U num)  { 
        for (size_t i = 0; i < size_; ++i) {
            cells_[i] = num;
        }
        return *this;
    }

    Matrix operator - () {
        Matrix mat(*this);
        for (size_t i = 0; i < M_; ++i) {
            for (size_t j = 0; j < N_; ++j) {
                if (i != j) {
                    mat.cells_[i*N_+j] = -mat.cells_[i*N_+j];
                }
            }
        }
        return mat;
    }

    template <typename U, typename UAlloc> requires Plusable<T, U>
    Matrix& operator += (const Matrix<U, UAlloc>& other) {
        assert((M_ == other.M_ && N_ == other.N_));
        for (size_t i = 0; i < size_; ++i) {
            cells_[i] += other.cells_[i];
        }
        return *this;
    }

    template <typename U> requires Plusable<T, U>
    Matrix& operator += (U num) {
        for (size_t i = 0; i < size_; ++i) {
            cells_[i] += num;
        }
        return *this;
    }

    template <typename U>
    Matrix operator + (const U& num) const {
        Matrix copy(*this);
        copy += num;
        return copy; // rvo + copy elision
    }

    template <typename U> requires Multiplyable<T, U>
    Matrix& operator *= (U num) {
        for (size_t i = 0; i < size_; ++i) {
            cells_[i] *= num;
        }
        return *this;
    }

    template <typename U, typename UAlloc> requires Multiplyable<T, U>
    Matrix& operator *= (const Matrix<U, UAlloc>& other) {
        *this = std::move(*this * other);
        return *this; // rvo + copy elision
    }

    template <typename U, typename UAlloc> requires Multiplyable<T, U>
    Matrix operator * (const Matrix<U, UAlloc>& other) const {
        assert(N_ == other.M_);
        size_t K = other.N_;
        Matrix result(M_, K);

        for (size_t i = 0; i < M_; ++i) {
            for (size_t k = 0; k < N_; ++k) {
                for (size_t j = 0; j < K; ++j) {
                    result.cells_[i*K + j] += cells_[i*N_ + k] * other.cells_[k*K + j];
                }
            }
        }
        return result; // rvo + copy elision
    }

    template <typename U> requires Multiplyable<T, U>
    Matrix operator * (U num) const {
        Matrix copy(*this);
        copy *= num;
        return copy; // rvo + copy elision
    }
    

    T* operator [] (size_t i) { return cells_+i*N_; }
    const T* operator [] (size_t i) const { return cells_+i*N_; }

    Matrix transpose() const noexcept {
        Matrix mat(N_, M_);
        for (size_t i = 0; i < M_; ++i) {
            for (size_t j = 0; j < N_; ++j) {
                mat.cells_[j*M_+i] = cells_[i*N_+j];
            }
        }
        return mat;
    }

    void resize(size_t new_size) {
        size_ = new_size;
        if (capacity_ >= new_size) return;
        size_t new_capacity = 2*new_size;

        T* new_cells = Alloc::allocate(alloc_, new_capacity);

        for (size_t i = 0; i < size_; ++i) {
            Alloc::construct(alloc_, new_cells+i, cells_[i]);
        }
        destroy_cells();

        cells_ = new_cells;
        capacity_ = new_capacity;
    }

    void appendRow(const Matrix& other) {
        assert(N_ == other.N_);
        size_t old_size = size_;
        size_t other_size = other.size_;
        resize(old_size + other.size_);

        for (size_t i = old_size; i < old_size+other_size; ++i) {
            Alloc::construct(alloc_, cells_+i, other.cells_[i-old_size]);
        }
        M_ += other.M_;
    }

    void appendCol(const Matrix& other) {
        assert(M_ == other.M_);

        resize(size_ + other.size_);

        *this = transpose();
        appendRow(other.transpose());
        *this = transpose();
    }

    size_t rows() const { return M_; }
    size_t cols() const { return N_; }

    static Matrix eye(size_t M, size_t N, T k = 1) {
        Matrix mat(M, N, T(0));
        for (size_t i = 0; i < M; ++i) {
            mat.cells_[i*N + i] = k;
        }
        return mat;
    }

    // #if M_ == N_ && M_ >= 2
    static Matrix rotate(size_t dimensions, double angle, size_t ax1=0, size_t ax2=1) {
        Matrix mat = Matrix::eye(dimensions, dimensions);
        double scale = std::cos(angle);
        double shift = std::sin(angle);
        if (ax1 > ax2 || (ax1 == 0 && ax2 == mat.M_-1)) std::swap(ax1, ax2);

        mat[ax1][ax1] = scale;
        mat[ax2][ax2] = scale;
        mat[ax1][ax2] = shift;
        mat[ax2][ax1] = -shift;
        
        return mat;
    }
    // #endif

    static Matrix rotate3dX(double angle) { return rotate(4, angle, 1, 2); }
    static Matrix rotate3dY(double angle) { return rotate(4, angle, 0, 2); }
    static Matrix rotate3dZ(double angle) { return rotate(4, angle, 0, 1); }

    friend std::ostream& operator << (std::ostream& out, const Matrix &mat) {
        out << "[";
        for (size_t i = 0; i < mat.M_; ++i) {
            for (size_t j = 0; j < mat.N_; ++j) {
                out << "\t" << mat[i][j];
            }
            out << ((i == mat.M_-1) ? "\t]" : "") << endl;
        }
        return out;
    }

private:
    T* cells_;
    size_t M_;  // rows
    size_t N_;  // cols
    size_t size_;
    size_t capacity_;
    Alloc_ alloc_ = Alloc_();
};


template <typename T, typename Alloc_, typename U> requires(std::is_arithmetic_v<U>)
Matrix<T, Alloc_> operator + (const U& a, Matrix<T, Alloc_> b) { b += a; return b; }

template <typename T, typename Alloc_, typename U> requires(std::is_arithmetic_v<U>)
Matrix<T, Alloc_> operator * (const U& a, Matrix<T, Alloc_> b) { b *= a; return b; }

template <typename Alloc_ = std::allocator<int>>
using Matrix_i = Matrix<int, Alloc_>;

template <typename Alloc_ = std::allocator<double>>
using Matrix_d = Matrix<double, Alloc_>;

void test_plus() {
    cout << " Testing + operation..." << endl;
    Matrix_i<> mat1(2, 3), mat2(2, 3);
    Matrix mat(2, 3);
    mat += 2.2;
    cout << mat << endl;
    mat += mat;
    mat + mat;         // 1 copy
    cout << mat << endl;
    mat = 1 + mat;     // 1 copy
    cout << mat << endl;
    mat = 0.1;         // 1 copy
    cout << mat << endl;

    mat2 = mat1;       // 1 copy
    mat += mat2;    // CE
    auto d = mat2 + mat;
    cout << mat2 << endl;
}

void test_multiply() {
    cout << " Testing * operation..." << endl;
    Matrix_i<> ai(2, 3, 4);
    Matrix ad(2, 3, 2.0);
    Matrix bd(3, 6, 3.0);
    cout << ad << endl;
    cout << bd << endl;
    auto cd = ad * bd;  // 0cp 1c
    cout << cd << endl; 
    ad *= 2.2;
    cout << ad << endl;
    ad = 2 * ad;       
    cout << ad << endl;
    ad = 0.1;         
    cout << ad << endl;
    ad *= bd;    
    cout << ad << endl;

    auto d = ai * bd;
    cout << d << endl;
}

void load_multiply_test() {
    Matrix a(1024, 1024), b(1024, 1024);
    Bench bench;
    size_t complexity = a.rows() * a.cols() * b.cols();

    for (size_t i = 0; i < a.rows(); ++i) {
        for (size_t j = 0; j < a.cols(); ++j) {
            a[i][j] = std::rand() / 10.0;
            b[i][j] = std::rand() / 10.0;
        }
    }

    bench.start();
    auto c = a * b;
    cout << " performance: " << complexity / bench.measure() << " op/s" << endl;
}

void test_append() {
    Matrix a = {
        {1, 2, 3},
        {3, 4, 5},
        {6, 7, 8},
        {9, 10, 11}
    };
    Matrix b = a;
    cout << a << endl;
    a.appendRow(a);
    cout << a << endl;
    a.appendRow(a);
    cout << a << endl;

    cout << b << endl;
    // b.appendCol({{1}, {2}, {3}, {4}}); // CE
    b.appendCol({{1,1}, {2,2}, {3,3}, {4,4}});
    cout << b << endl;

}

void test_brackets() {
    cout << " Testing [] operation..." << endl;
    Matrix mat(3, 3);
    cout << mat << endl;
    mat[0][1] = 1;
    cout << mat << endl;
}

#endif