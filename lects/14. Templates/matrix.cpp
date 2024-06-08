#include <iostream>
#include <string>
#include "matrix.hpp"

using std::cout;
using std::cin;
using std::endl;

using std::ostream;

// template <size_t N, size_t M>
// Matrix<N, M>::Matrix(double init=0.0) {
//     mat = new double[N*M];
//     // memset(mat, init, sizeof(mat)*N*M);
//     for (size_t i = 0; i < N; ++i) {
//         for (size_t j = 0; j < M; ++j)
//             mat[i*M + j] = init;
//     }
// }

template <size_t N, size_t M>
double* Matrix<N, M>::operator [] (const size_t &i) {
    return &(mat[i*M]);
}

template <size_t N, size_t M>
const double* Matrix<N, M>::operator [] (const size_t &i) const {
    return &(mat[i*M]);
}

template <size_t N, size_t M>
Matrix<N, M>::~Matrix() {
    delete [] mat;
}

template <size_t N, size_t M>
ostream& operator << (ostream &out, const Matrix<N, M> &m) {
    // out << "[";
    for (size_t i = 0; i < N; ++i) {
        out << "[ ";
        for (size_t j = 0; j < M; ++j)
            printf("%-6.2lf ", m[i][j]);
        out << "]";
        if (i != N-1) 
            out << '\n';
    }
    out << "\n";
    return out;
}

template <size_t N, size_t M>
void Matrix<N, M>::swap(Matrix<N, M> &m) {
    std::swap(mat, m.mat);
}

template <size_t N, size_t M>
Matrix<N, M>::Matrix(const Matrix<N, M> &m) : Matrix<N, M>() {
    cout << "c";
    memcpy(mat, m.mat, sizeof(*mat)*M*N);
}

template <size_t N, size_t M>
Matrix<N, M>& Matrix<N, M>::operator = (Matrix<N, M> m) {
    swap(m);
    return *this;
}

template <size_t N, size_t M>
Matrix<N, M>& Matrix<N, M>::operator += (const Matrix<N, M> &m) {
    for (size_t i = 0; i < M*N; ++i)
        mat[i] += m.mat[i];
    return *this;
}

template <size_t N, size_t M> 
Matrix<N, M> Matrix<N, M>::operator + (const Matrix<N, M> &m) const {
    Matrix<N, M> copy(*this);
    copy += m;
    return copy;
}

template <size_t N, size_t M> 
Matrix<N, M> Matrix<N, M>::operator - () const {
    Matrix<N, M> m(*this);
    for (size_t i = 0; i < M*N; ++i)
        m.mat[i] = -m.mat[i];
    return m;
}

template <size_t N, size_t M>
Matrix<N, M>& Matrix<N, M>::operator -= (const Matrix<N, M> &m) {
    *this += -m;
    return *this;
}

template <size_t N, size_t M> 
Matrix<N, M> Matrix<N, M>::operator - (const Matrix<N, M> &m) const {
    Matrix<N, M> copy(*this);
    copy -= m;
    return copy;
}

template <size_t N, size_t M, size_t K>
Matrix<N, K>& Matrix<N, M>::operator *= (const Matrix<M, K> &m) {

}

// template <size_t N, size_t M> 
// // не работает!!!!
// Matrix<N, M> operator + (const Matrix<N, M> &m1, const Matrix<N, M> &m2) {
//     Matrix<N, M> copy(m1);
//     copy += m2;
//     return copy;
// }

template <size_t N, size_t M> 
Matrix<N, M> operator + (const double &m1, const Matrix<N, M> &m2) {
    Matrix<N, M> copy(m1);
    copy += m2;
    return copy;
}

// template <size_t N, size_t M> 
// // не работает!!!!
// Matrix<N, M> operator - (const Matrix<N, M> &m1, const Matrix<N, M> &m2) {
//     Matrix<N, M> copy(m1);
//     copy -= m2;
//     return copy;
// }

template <size_t N, size_t M> 
Matrix<N, M> operator - (const double &m1, const Matrix<N, M> &m2) {
    Matrix<N, M> copy(m1);
    copy -= m2;
    return copy;
}

template <size_t N, size_t M>
Matrix<N, M> Matrix<N, M>::eye() {
    Matrix<N, M> m();

    for (size_t i = 0; i < std::min(M, N); ++i)
        m[i][i] = 1.0;

    return m;
}