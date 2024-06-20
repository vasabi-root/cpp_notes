#include <iostream>
#include <vector>
// #include <type_traits>

using std::cin;
using std::cout;
using std::endl;

template <typename T>
struct type_identity {
    using type = T;
};

template <typename T>
using type_identity_t = typename type_identity<T>::type; // c++20: можно без typename


template <typename T, T x>
struct integral_constant {
    static constexpr T value = x;
};

template <bool b>
using bool_constant = integral_constant<bool, b>;

using true_type = bool_constant<true>;
using false_type = bool_constant<false>;

template <typename T>
struct is_lvalue_ref: false_type {};

template <typename T>
struct is_lvalue_ref<T&>: true_type {};

template <typename T>
using is_lvalue_ref_v = typename is_lvalue_ref<T>::value;

// ------ conjunction --------------------------------------------------

template <typename... Types>
struct conjunction {
    static constexpr bool value = (Types::value && ...);
};

// ---- SFINAE ---------------------------------------------------------

template <typename T, typename = T::value_type>
void f(T) {
  cout << "1" << endl;
}

// template <typename T>
// typename T::reference f(T) { // call to 'f' is ambiguous
//     cout << "3" << endl;
//     return {};
// }

template <typename... Args>
void f(Args...) {
    cout << "2" << endl;
}
template <typename... Args>
bool is_integral_type(Args...) {return false;}

template <typename T>
auto is_integral_type(T x) -> decltype(x%2) {return 1;}

// ----------------------------------------------------------------------

namespace my1 {

template <bool b>
struct enable_if {};

template<>
struct enable_if<true> {
  using type = void;
};

template<bool b>
using enable_if_v = enable_if<b>::type;

template <typename T, typename = enable_if_v<std::is_integral_v<T>> >
void g(T x) {
  cout << "integral" << endl; 
}

template <typename... Args>
void g(Args...) {
  cout << "not integral" << endl; 
}

}
// Избавляемся от Args ---------------------------------------------------------
namespace my2 {

template <bool b, typename T>
struct enable_if {};

template<typename T>
struct enable_if<true, T> {
  using type = T;
};

template<bool b, typename T=void>
using enable_if_v = enable_if<b, T>::type;

template <typename T, enable_if_v<std::is_integral_v<T>, bool> = true>
void g(T x) {
  cout << "integral" << endl; 
}

template <typename T, enable_if_v<!std::is_integral_v<T>, bool> = true> // неважно, чему равно bool
void g(T x) {
  cout << "not integral" << endl; 
}

// template <typename T, typename = enable_if_v<!std::is_integral_v<T>> > // CE!!!
// void g(T x) {
//   cout << "not integral" << endl; 
// }

}

// --- is_class ------------------------------------------
namespace my3 {

template <typename T>
true_type is_class_f(int T::*); // указатель на член типа int (его может и не быть 
                                // в классе, но указатель может спокойно существовать)
template <typename>
false_type is_class_f(...); // C-style multyargs

template <typename T>
struct is_class: decltype(is_class_f<T>(nullptr)) {};

template <typename T>
inline constexpr bool is_class_v = is_class<T>::value;
}

// --- is_polymorphic ------------------------------------------
namespace my3 {

template <typename T>
true_type is_poly_f( decltype(dynamic_cast<const void*>(static_cast<T*>(nullptr))) );

template <typename>
false_type is_poly_f(...); // C-style multyargs

template <typename T>
struct is_polymorphic: decltype(is_poly_f<T>(nullptr)) {};

template <typename T>
inline constexpr bool is_polymorphic_v = is_polymorphic<T>::value;
}

struct Ss { virtual ~Ss() {} };

int main() { 
    type_identity_t<int> a;
    // cout << conjunction<true_type, true_type>::value << endl;

    std::vector<int> v(1);

    f(v); 
    f(0);
    cout << is_integral_type(0.4) << endl;
    my1::g(0.8);
    my1::g(char(1));
    my2::g(0.8);
    my2::g(0);

    cout << my3::is_class_v<int> << endl;
    cout << my3::is_class_v<Ss> << endl;

    cout << my3::is_polymorphic_v<int> << endl;
    cout << my3::is_polymorphic_v<Ss> << endl;
    return 0;
}


// Костыльно можно через decltype() реализовать различное поведение для целочисленного аргумента и всех остальных:

// template  <typename... Args>
// void f(Args...) {
//     std::cout << "not integral" << std::endl; 
//  }

// template <typename T>
// auto f(T x) -> decltype(x%2) {
//     std::cout << "integral" << std::endl; 
//     return {};
// }