#include <iostream>
#include <vector>
#include <string>
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
using enable_if_t = enable_if<b>::type;

template <typename T, typename = enable_if_t<std::is_integral_v<T>> >
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
using enable_if_t = enable_if<b, T>::type;

template <typename T, enable_if_t<std::is_integral_v<T>, bool> = true>
void g(T x) {
  cout << "integral" << endl; 
}

template <typename T, enable_if_t<!std::is_integral_v<T>, bool> = true> // неважно, чему равно bool
void g(T x) {
  cout << "not integral" << endl; 
}

// template <typename T, typename = enable_if_t<!std::is_integral_v<T>> > // CE!!!
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

// ------- has_construct -----------------------------------------

namespace my4 {

template <typename T>
T&& declval() noexcept; // на самом деле, тут вообще  std::add_rvalue_reference, чтоб работало для void

template<typename T, typename... Args>
true_type has_construct_f( decltype(declval<T>().construct(declval<Args>()...), nullptr ));

template <typename...>
false_type has_construct_f(...);

template <typename T, typename... Args>
struct has_construct: decltype(has_construct_f<T, Args...>(nullptr)) {};

template <typename T, typename... Args>
constexpr bool has_construct_v = has_construct<T, Args...>::value;

struct A;
struct S {
    int construct(int, float);
    void construct(int, int, int);
    void construct(A&&);
    void construct();
};

}

// --- has_move_if_noexcept --------------------------------------------

namespace my5 {

template <typename T>
true_type has_move_if_noexcept_f(my2::enable_if_t<noexcept(T(my4::declval<T>())), decltype(nullptr)>);

template <typename...>
false_type has_move_if_noexcept_f(...);

template <typename T>
struct has_move_if_noexcept: decltype(has_move_if_noexcept_f<T>(nullptr)) {};

template <typename T>
constexpr bool has_move_if_noexcept_v = has_move_if_noexcept<T>::value;

struct Good {
  Good(Good&&) noexcept {};
};

struct Bad {
  Bad(Bad&&) {};
};

struct VeryBad {
  VeryBad(VeryBad&&) = delete;
};

}


// -------- is_base_of (only public) ----------------------------------------------------------

namespace my6 {

template <typename B, typename D>
true_type is_base_of_f(decltype(B(my4::declval<D>()), nullptr));

template <typename ...>
false_type is_base_of_f(...);

template <typename B, typename D>
struct is_base_of : decltype(is_base_of_f<B, D>(nullptr)) {};

template <typename B, typename D>
constexpr bool is_base_of_v = is_base_of<B, D>::value;


struct B{};
struct D1:B {};
struct D2: private B {};

struct G {};
struct M: virtual G {};
struct F: virtual G {};
struct S: M, F {};
}

// -------- is_base_of (public & private) ----------------------------------------------------------

namespace my7 {

template <typename B>
true_type cast_trick(const B*);

template <typename ...>
false_type cast_trick(...);

template <typename B, typename D>
auto is_base_of_f(int) -> decltype(cast_trick<B>(my4::declval<D*>()));

template <typename, typename>
auto is_base_of_f(...) -> true_type; // private & ambiguous

template <typename B, typename D>
struct is_base_of : std::conjunction<
  std::is_class<B>, std::is_class<D>,
  decltype(is_base_of_f<B, D>(0))
> {};

template <typename B, typename D>
constexpr bool is_base_of_v = is_base_of<B, D>::value;

struct B{};
struct D1:B {};
struct D2: private B {};

struct G {};
struct M:G {};
struct F:G {};
struct S: M, F {};
}

// ------ common_type ------------------------------------------------
namespace my8 {

template <typename... Tail>
struct common_type;

template <typename T>
struct common_type<T>: std::type_identity<T> {};

template <typename T, typename U>
struct common_type<T, U>: std::type_identity<
  std::decay_t<decltype(true ? my4::declval<T>() : my4::declval<U>())>
> {};

template <typename Head, typename... Tail>
struct common_type<Head, Tail...>: std::type_identity<
  typename common_type<Head, typename common_type<Tail...>::type>::type
> {};
template <typename...>
void foo(...) {};

template <typename T>
void foo(T x, typename common_type<T, int>::type y) {}


template <typename...>
void bar(...) {};

template <typename T>
void bar(T x, typename std::common_type_t<T, int> y) {}


struct B{};
struct D1:B {};
struct D2: private B {};

struct G {};
struct M:G {};
struct F:G {};
struct S: M, F {};
}

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

    static_assert(!my4::has_construct_v<int>);
    static_assert(my4::has_construct_v<my4::S, int, int, int>);
    static_assert(my4::has_construct_v<my4::S, int, float>);
    static_assert(my4::has_construct_v<my4::S, my4::A>);
    static_assert(my4::has_construct_v<my4::S>);

    static_assert(my5::has_move_if_noexcept_v<my5::Good>);
    static_assert(!my5::has_move_if_noexcept_v<my5::Bad>);
    static_assert(!my5::has_move_if_noexcept_v<my5::VeryBad>);

    static_assert(my6::is_base_of_v<my6::B, my6::D1>);
    // static_assert(my6::is_base_of_v<my6::B, my6::D2>); // CE
    static_assert(my6::is_base_of_v<my6::G, my6::M>);
    static_assert(my6::is_base_of_v<my6::M, my6::S>);
    static_assert(my6::is_base_of_v<my6::F, my6::S>);
    static_assert(my6::is_base_of_v<my6::G, my6::S>); // CE if non virtual
    static_assert(!my6::is_base_of_v<my6::B, my6::S>);
    static_assert(!my6::is_base_of_v<my6::B, int>);

    static_assert(my7::is_base_of_v<my7::B, my7::D1>);
    static_assert(my7::is_base_of_v<my7::B, my7::D2>);
    static_assert(my7::is_base_of_v<my7::G, my7::M>);
    static_assert(my7::is_base_of_v<my7::M, my7::S>);
    static_assert(my7::is_base_of_v<my7::F, my7::S>);
    static_assert(my7::is_base_of_v<my7::G, my7::S>); 
    static_assert(!my7::is_base_of_v<my7::B, my7::S>);
    static_assert(!my7::is_base_of_v<my7::B, int>);

    static_assert( std::is_same_v<my8::common_type<int, double, float>::type, double>);
    static_assert( std::is_same_v<my8::common_type<int>::type, int>);
    static_assert( std::is_same_v<my8::common_type<int, int>::type, int>);
    static_assert( std::is_same_v<my8::common_type<my8::B, my8::D1>::type, my8::B>);
    // static_assert( std::is_same_v<my8::common_type<my8::B, my8::D2>::type, my8::B>); // CE
    static_assert( std::is_same_v<my8::common_type<my8::G, my8::M>::type, my8::G>); 
    // static_assert( std::is_same_v<std::common_type_t<my8::B, my8::D2>, my8::B>); // CE
    // static_assert( std::is_same_v<std::common_type_t<my8::G, my8::S>, my8::G>); // CE
    
    my8::bar<std::string>(std::string("hello"), std::string("world"));
    my8::foo<std::string>(std::string("hello"), std::string("world"));
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