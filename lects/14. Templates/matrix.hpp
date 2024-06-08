#include <iostream>
#include <cstring>

using std::cout;
using std::cin;
using std::endl;

using std::ostream;

template <size_t N, size_t M>
class Matrix {
private:
    double *mat = nullptr;
    size_t n_ = N;
    size_t m_ = M;

public:
    Matrix(const double &init = 0.0) {
        mat = new double[N*M];
        for (size_t i = 0; i < N*M; ++i)
            mat[i] = init;
        // memset(mat, init, sizeof(*mat)*N*M);
    }
    Matrix(const Matrix &);

    ~Matrix();

    double *operator [] (const size_t &i);
    const double* operator [] (const size_t &i) const;

    Matrix& operator = (Matrix m);

    Matrix& operator += (const Matrix &m);
    Matrix operator + (const Matrix &m) const;
    Matrix operator - () const;
    Matrix& operator -= (const Matrix &m);
    Matrix operator - (const Matrix &m) const;

    template <size_t K>
    Matrix<N, K>& operator *= (const Matrix<M, K> &m);


    void swap(Matrix &m);
    
    static Matrix eye();

    // friend ostream& operator << (ostream &out, const Matrix<N, M> &m);
};