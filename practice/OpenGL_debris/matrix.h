#ifndef __PH_MATRIX_H__
#define __PH_MATRIX_H__

#include <iostream>
#include <cmath>
#include <fstream>
#include <windows.h>
#include "../bench.h"

using std::cin;
using std::cout;
using std::endl;

template <typename T, typename U> 
concept Plusable = requires (T t, U u)  { t += u; };

template <typename T, typename U> 
concept Multiplyable = requires (T t, U u)  { t *= u; };

// T requires to have T(), T(1)

template <
    size_t M,  // rows
    size_t N,  // cols
    typename T = double,  
    typename Alloc_ = std::allocator<T>
>
requires (std::__is_allocator<Alloc_>::value)
class Matrix {
public:
    using Alloc = std::allocator_traits<Alloc_>; //::rebind_traits<T*>

    template <typename U>
    using RebindAlloc = typename std::allocator_traits<Alloc_>::template rebind_alloc<U>;

    template <size_t M2, size_t N2, typename T2, typename Alloc2_> 
    requires (std::__is_allocator<Alloc2_>::value)
    friend class Matrix ;

    template <typename... Args>
    Matrix(const Args&... args): cells_(Alloc::allocate(alloc_, M*N)) {
        for (size_t i = 0; i < size_; ++i) {
            Alloc::construct(alloc_, cells_+i, args...);
        }
    }

    Matrix (std::initializer_list<T> list): cells_(Alloc::allocate(alloc_, M*N)) {
        for (size_t i = 0; i < size_; ++i) {
            Alloc::construct(alloc_, cells_+i, 1);
            if (i < list.size()) {
                cells_[i] = *(list.begin()+i);
            }
        }
    }

    Matrix(const Matrix& other): cells_(Alloc::allocate(alloc_, M*N)) {
        for (size_t i = 0; i < size_; ++i) {
            Alloc::construct(alloc_, cells_+i, other.cells_[i]);
        }
    }

    Matrix(Matrix&& other) { //: cells_(Alloc::allocate(alloc_, M*N)) {
        std::swap(cells_, other.cells_);
        other.cells_ = nullptr;
    }

    Matrix& operator = (const Matrix &other) {
        Matrix copy(other);
        std::swap(cells_, copy.cells_);
        copy.cells_ = nullptr;
        return *this;
    }

    Matrix& operator = (Matrix&& other) { 
        std::swap(cells_, other.cells_);
        other.cells_ = nullptr;
        return *this;
    }

    template <typename U, typename UAlloc> requires Plusable<T, U>
    Matrix& operator += (const Matrix<M, N, U, UAlloc>& other) {
        for (size_t i = 0; i < size_; ++i) {
            cells_[i] += other.cells_[i];
        }
        return *this;
    }

    template <typename U> requires Plusable<T, U>
    Matrix& operator += (U other) {
        for (size_t i = 0; i < size_; ++i) {
            cells_[i] += other;
        }
        return *this;
    }

    template <typename U>
    Matrix operator + (const U& num) {
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

    template <size_t K, typename U, typename UAlloc> requires Multiplyable<T, U>
    auto operator * (const Matrix<N, K, U, UAlloc>& other) {
        using V = std::common_type_t<T, U>;
        Matrix<M, K, V, RebindAlloc<V>> result;

        for (size_t i = 0; i < M; ++i) {
            for (size_t k = 0; k < N; ++k) {
                for (size_t j = 0; j < K; ++j) {
                    result.cells_[i*K + j] += cells_[i*N + k] * other.cells_[k*K + j];
                }
            }
        }
        return result; // rvo + copy elision
    }

    template <typename U> requires Multiplyable<T, U>
    Matrix operator * (const U& num) {
        Matrix copy(*this);
        copy *= num;
        return copy; // rvo + copy elision
    }
    

    T* operator [] (size_t i) { return cells_+i*N; }
    const T* operator [] (size_t i) const { return cells_+i*N; }

    ~Matrix() { 
        if (cells_) {
            for (size_t i = 0; i < size_; ++i) { 
                Alloc::destroy(alloc_, cells_+i);
            }
            Alloc::deallocate(alloc_, cells_, size_);
        }
    }

    #if N == 1 || M == 1
    T norm() {
        T val;
        for (size_t i = 0; i < size_; ++i) {
            val += cells_[i]*cells_[i];
        }
        return std::sqrt(val);
    }
    #endif

    size_t rows() { return M; }
    size_t cols() { return N; }

    static Matrix eye(T k = 1) {
        Matrix mat;
        for (size_t i = 0; i < M; ++i) {
            mat.cells_[i*N + i] = k;
        }
        return mat;
    }

    // #if M == N && M >= 2
    static Matrix rotate(double angle, size_t ax1=0, size_t ax2=1) {
        Matrix mat = Matrix::eye();
        double scale = std::cos(angle);
        double shift = std::sin(angle);
        if (ax1 > ax2 || ax1 == 0 && ax2 == M-1) std::swap(ax1, ax2);

        mat[ax1][ax1] = scale;
        mat[ax2][ax2] = scale;
        mat[ax1][ax2] = -shift;
        mat[ax2][ax1] = shift;
        
        return mat;
    }
    // #endif

    static Matrix rotateX(double angle) { return rotate(angle, 1, 2); }
    static Matrix rotateY(double angle) { return rotate(angle, 0, 2); }
    static Matrix rotateZ(double angle) { return rotate(angle, 0, 1); }

    friend std::ostream& operator << (std::ostream& out, const Matrix &mat) {
        out << "[";
        for (size_t i = 0; i < M; ++i) {
            for (size_t j = 0; j < N; ++j) {
                out << "\t" << mat[i][j];
            }
            out << ((i == M-1) ? "\t]" : "") << endl;
        }
        return out;
    }

private:
    T* cells_;
    size_t size_ = M*N;
    Alloc_ alloc_ = Alloc_();
};


template <size_t M,  size_t N, typename T, typename Alloc_, typename U>
Matrix<M, N, T, Alloc_> operator + (const U& a, Matrix<M, N, T, Alloc_> b) { b += a; return b; }

template <size_t M,  size_t N, typename T, typename Alloc_, typename U>
Matrix<M, N, T, Alloc_> operator * (const U& a, Matrix<M, N, T, Alloc_> b) { return b * a; }

template <size_t M,  size_t N, typename Alloc_ = std::allocator<int>>
using MatrixInt = Matrix<M, N, int, Alloc_>;

template <typename Alloc_ = std::allocator<double>>
using Matrix3d = Matrix<4, 4, double, Alloc_>;

template <typename Alloc_ = std::allocator<double>>
using Vec3d_f = Matrix<4, 1, double, Alloc_>;

template <typename Alloc_ = std::allocator<int>>
using Vec3d_i = Matrix<4, 1, int, Alloc_>;

void test_plus() {
    cout << " Testing + operation..." << endl;
    MatrixInt<2, 3> mat1, mat2;
    Matrix<2, 3, double> mat;
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
    cout << mat2 << endl;
}

void test_multiply() {
    cout << " Testing * operation..." << endl;
    MatrixInt<2, 3> ai(4);
    Matrix<2, 3> ad(2.0);
    Matrix<3, 6> bd(3.0);
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
    // ad *= mat2;    // CE 

    auto d = ai * bd;
    cout << d << endl;
}

void load_multiply_test() {
    Matrix<1024, 1024> a, b;
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

void test_brackets() {
    cout << " Testing [] operation..." << endl;
    Matrix<3, 3> mat;
    cout << mat << endl;
    mat[0][1] = 1;
    cout << mat << endl;
}

#endif