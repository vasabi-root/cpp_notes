#include <iostream>
#include <cmath>
#include <fstream>

using std::cin;
using std::cout;
using std::endl;

template <
    size_t M,  // rows
    size_t N,  // cols
    typename T,  
    typename Alloc_ = std::allocator<T>
>
requires (std::__is_allocator<Alloc_>::value)
class Matrix {
public:
    using Alloc = std::allocator_traits<Alloc_>; //::rebind_traits<T*>

    template <typename... Args>
    Matrix(const Args&... args): cells_(Alloc::allocate(alloc_, M*N)) {
        for (size_t i = 0; i < size_; ++i) {
            Alloc::construct(alloc_, cells_+i, args...);
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

    Matrix& operator = (Matrix other) {
        std::swap(cells_, other.cells_);
        other.cells_ = nullptr;
        return *this;
    }

    Matrix& operator = (Matrix&& other) { 
        std::swap(cells_, other.cells_);
        other.cells_ = nullptr;
        // cells_ = Alloc::allocate(alloc_, M*N);
        // for (size_t i = 0; i < size_; ++i) {
        //     Alloc::construct(alloc_, cells_+i, std::move(other.cells_[i]));
        //     Alloc::destroy(alloc_, other.cells_+i);
        // }
        // Alloc::deallocate(other.alloc_, other.cells_, other.size_);
        // other.cells_ = nullptr;

        return *this;
    }

    Matrix& operator += (const Matrix& other) {
        for (size_t i = 0; i < size_; ++i) {
            cells_[i] += other.cells_[i];
        }
    }

    Matrix operator + (const Matrix& other) {
        Matrix copy(*this);
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

template <
    size_t M,  // rows
    size_t N,  // cols
    typename Alloc_ = std::allocator<int>
>
using MatrixInt = Matrix<M, N, int, Alloc_>;

int main(int argc, char** argv) {
    MatrixInt<2, 3> mat1, mat2;
    MatrixInt<2, 3> mat3(mat1);
    MatrixInt<2, 3> mat4(std::move(mat2));
    mat2 = mat1;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << mat2 << endl;
    mat2[0][1] = 100;
    cout << mat2 << endl;
    
}