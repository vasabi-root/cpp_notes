#ifndef __PH_VEC_H__
#define __PH_VEC_H__

#include <iostream>
#include "matrix.h"
#include <cassert>
#include <vector>

template <
    size_t N,
    typename T, 
    typename Alloc_ = std::allocator<T>
>
requires (std::__is_allocator<Alloc_>::value)
class Vec: public Matrix<T, Alloc_> {
public:
    using Mat = Matrix<T, Alloc_>;

    using Matrix<T, Alloc_>::Matrix;
    using Matrix<T, Alloc_>::operator *;

    Vec(): Mat(1, N) {}
    Vec(std::initializer_list<T> list): Mat(1, N, T(1)) {
        assert(list.size() <= N);
        for (size_t i = 0; i < list.size(); ++i) {
            (*this)[0][i] = *(list.begin()+i);
        }
    }
};


template <size_t N, typename Alloc = std::allocator<double>> 
using Vec_d = Vec<N, double, Alloc>;

template <typename Alloc = std::allocator<double>> 
using Vec3_d = Vec<3, double, Alloc>;

template <typename Alloc = std::allocator<double>> 
using Vec4_d = Vec<4, double, Alloc>;


template <class T> struct Vec4 {
	union {
		struct {T x, y, z, s;};
		struct { T ivert, iuv, inorm, scale; };
		T raw[4];
	};
	Vec4() : x(0), y(0), z(0), s(1) {}
	Vec4(T x, T y, T z=1, T s=1) : x(x),y(y),z(z),s(s) {}
	inline Vec4<T> operator ^ (const Vec4<T> &v) const { return Vec4<T>(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x); }
	inline Vec4<T> operator + (const Vec4<T> &v) const { project3d(); v.project3d(); return Vec4<T>(x+v.x, y+v.y, z+v.z); }
	inline Vec4<T> operator - (const Vec4<T> &v) const { project3d(); v.project3d(); return Vec4<T>(x-v.x, y-v.y, z-v.z); }
	inline Vec4<T> operator * (float f)          const { return Vec4<T>(x*f, y*f, z*f); }
	inline T       operator * (const Vec4<T> &v) const { return (x*v.x + y*v.y + z*v.z) / float(s*v.s); }
	float norm () const { return std::sqrt(x*x+y*y+z*z); }
    Vec4 & project3d() { *this = *this*(1.0f/s); return *this; }
    Vec4 & project2d() { *this = *this*(1.0f/z); return *this; }
	Vec4 & normalize(T l=1) { *this = (*this)*(l/norm()); return *this; }

    template <class U, class Alloc = std::allocator<U>> requires (Multiplyable<T, U>)
    Vec4<T>       operator * (const Matrix<U, Alloc> &mat) const { 
        assert(mat.rows() == 4 && mat.rows() == mat.cols());
        return {
            static_cast<T>(x*mat[0][0] + y*mat[1][0] + z*mat[2][0] + s*mat[3][0]),
            static_cast<T>(x*mat[0][1] + y*mat[1][1] + z*mat[2][1] + s*mat[3][1]),
            static_cast<T>(x*mat[0][2] + y*mat[1][2] + z*mat[2][2] + s*mat[3][2]),
            static_cast<T>(x*mat[0][3] + y*mat[1][3] + z*mat[2][3] + s*mat[3][3]),
        };
    }
    template <class U, class Alloc = std::allocator<U>> requires (Multiplyable<T, U>)
    inline Vec4 & operator *= (const Matrix<U, Alloc> &mat) { *this = *this * mat; return *this; }

	template <class> friend std::ostream& operator<<(std::ostream& s, Vec4<T>& v);
};

typedef Vec4<float> Vec4f;
typedef Vec4<int>   Vec4i;

template <class T> std::ostream& operator<<(std::ostream& out, const Vec4<T>& v) {
	out << "(\t" << v.x << "\t" << v.y << "\t" << v.z << "\t" << v.s << "\t)\n";
	return out;
}

template <class T> std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {

    for (const auto & el: v) {
        out << " " << el;
    }
    return out;
}

template <class T, class U, class Alloc = std::allocator<U>> requires(Assignable<T, U>)
std::vector<Vec4<T>> operator * (const std::vector<Vec4<T>>& vec, const Matrix<U, Alloc> &mat) {
    auto res = vec;
    for (auto & v: res) {
        v *= mat;
    }
    return res;
}


void test_vec() {

    Vec4_d<> v1 = {1, 2, 3, 1};
    cout << v1 << endl;

    Vec4_d<> v2 = {1, 2, 3};
    cout << v2 << endl;

    Vec4_d<> v3;
    cout << v3 << endl;

    double angle = 60 * 3.1415;
    auto rotator = Matrix<>::rotate3dX(angle) 
                 * Matrix<>::rotate3dY(angle)
                 * Matrix<>::rotate3dZ(angle);
    
    Vec4_d<> v4 = {3, 3, 3};
    cout << v4 * rotator;
}


void test_vec4() {

    Vec4f v1 = {1, 2, 3, 1};
    cout << v1 << endl;

    Vec4f v2 = {1, 2, 3};
    cout << v2 << endl;

    Vec4f v3;
    cout << v3 << endl;

    double angle = 60 * 3.1415;
    auto rotator = Matrix<>::rotate3dX(angle) 
                 * Matrix<>::rotate3dY(angle)
                 * Matrix<>::rotate3dZ(angle);
    
    Vec4<double> v4 = {3, 3, 3};
    cout << v4 * rotator << endl;

    std::vector<Vec4f> vec = {
        {1, 2, 3}, 
        {3, 3, 3},
    };

    cout << vec * rotator << endl;
}


#endif