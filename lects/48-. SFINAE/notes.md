# SFINAE
## Вспоминаем
Мета-функции -- это шаблонные классы со статическими полями. Например, тождественная мета-функция:
```c++
template <typename T>
struct type_identity {
    using type = T;
};

template<typename T>
using type_identity_t = typename type_identity<T>::type; // c++20: можно без typename
```
Удобно использовать `static_assert()` для проверки условий в CT (но аргументы должны быть константами CT)

Или мета-класс для определения констант:
```c++ 
template <typename T, T x>
struct integral_constant {
    static constexpr T value = x;
};

template <bool b>
using bool_constant = integral_constant<bool, b>;

using true_type = bool_constant<true>;
using false_type = bool_constant<false>;
```

Тогда можно написать `type_traits`:
```c++
template <typename T>
struct is_lvalue_ref: false_type {};

template <typename T>
struct is_lvalue_ref<T&>: true_type {};
```

## Конъюнкция bool_constant наследников
`std::conjunction` -- конъюнкция бул-типов. Реализовано через [fold expressions](https://en.cppreference.com/w/cpp/language/fold) [c++17]
```c++
template <typename... Types>
struct conjunction {
    static constexpr bool value = (Types::value && ...);
};
```

НО! В стандарте указано, что если встретился тип `false` то для остальных необходимо прервать подстановку шаблонов. Поэтому корректная реализация будет содержать `Head Tail`

## SFINAE and enable_if
Substitution failure is not an error!
```c++
template <typename T>
typename T::value_type f(T) {
    cout << "1" << endl;
    return {};
}

template <typename... Args>
void f(Args...) {
    cout << "2" << endl;
}

int main() {
  f(std::vector<int>(0)); // 1
  f(0); // 2
}
```
Интересность в том, что шаблонные кандидаты вызываемой функции выбираются в соответствии с **объявлением**. То есть, на выбор влияет тип аргумента и тип возвращаемого значения, только если тип возвращаемого значения зависит от типа аргумента. При этом нельзя создать 2 нешаблонные функции с одинаковыми параметрами и разным возвратом -- поэтому если перегрузить шаблонную функцию для типов с разными полями и есть тип, который содержит в себе оба поля, то будет CE. Иначе можно натворить магию. 

В случае с первой функцией, мы обязали аргумент иметь внутренний тип `value_type`. Но если сделать ещё одну перегрузку с полем, которое есть у вектора, то будет CE:
```c++
template <typename T>
typename T::value_type f(T) {
    cout << "1" << endl;
    return {};
}

template <typename T>
typename T::reference f(T) { // call to 'f' is ambiguous
    cout << "3" << endl;
    return {};
}
```
Можно и по-другому оформить корректное решение:
```c++
template <typename T, typename = T::value_type>
void f(T) {
  cout << "1" << endl;
}
```
Как реализовать f, которая при целочисленном типе вела себя одним образом, а при других типах -- другим? Перечисление запрещено, но можно юзать `is_integral_v<T>`
```c++
template  <typename... Args>
void g(Args...) {
    cout << "not integral" << endl; 
 }

template <typename T>
auto g(T x) -> decltype(x%2) {
    cout << "integral" << endl; 
    return {};
}
```
Но это костыльно, лучше так:
```c++
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
```
Попробуем избавиться от Args. 

Нельзя просто взять и применить `enable_if_v<!std::is_integral_v<T>> >` к старой перегрузке -- это всё равно, что перегрузить функцию с теми же аргументами, но с другим аргументом по умолчанию -- CE. 

Нам надо пересмотреть реализацию `enable_if` -- добавить ей ещё один шаблонный тип, который она будет в себе хранить, в случае `true`. Тогда, в случае `false` при обращении к `type` имеем невалидный тип, а при `true` -- имеем объявление шаблонной переменной типа `bool`. И тут уже неважно, чем мы её инициализируем по умолчанию -- главное, что в валидном случае будет `bool`.
```c++
template <bool b, typename T>
struct enable_if {}; 

template<typename T>
struct enable_if<true, T> {
  using type = T;
};

template<bool b, typename T>
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
```
Так же не получилось бы нормально вывести тип шаблона, если бы во втором случае было что-то типа `enable_if_v<std::is_same_v<T, int>, bool> = true`, ибо так имеем `is_same_v<true, bool> = true` во время подстановки, что в точности совпадает с первой перегрузкой => CE.

## is_class
```c++
template <typename T>
true_type is_class_f(int T::*); // указатель на член типа int (его может и не быть 
                                // в классе, но указатель может спокойно существовать)
template <typename>
false_type is_class_f(...); // C-style multyargs

template <typename T>
struct is_class: decltype(is_class_f<T>(nullptr)) {};

template <typename T>
inline constexpr bool is_class_v = is_class<T>::value;
```

## is_polymorphic
dynamic_cast обычно работает в RT, НО! Он выдаёт CE, если тип неполиморфный  
Также разрешён каст к `void*` и `nul lptr` к T*.
```c++
template <typename T>
// const обязательно! Иначе будем кастить константу к неконстанте
true_type is_poly_f( decltype(dynamic_cast<const void*>(static_cast<T*>(nullptr))) );

template <typename>
false_type is_poly_f(...); // C-style multyargs

template <typename T>
struct is_polymorphic: decltype(is_poly_f<T>(nullptr)) {};
```

## has_method
Указатель на метод нельзя -- только если мы хотим убедиться, что класс имеет метод с такой сигнатурой, но не для того, чтоб понят, есть ли метод с таким-то именем! Делаем через `declval<T>()`. Также тут юзается `comma-trick`, чтобы тип аргумента функции бвл корректен.
```c++
template <typename T>
T&& declval(); // универсальная ссылка важна! иначе не будет работать для rvalue-ссылок

template<typename T, typename... Args>
true_type has_construct_f( decltype(declval<T>().construct(declval<Args>()...), nullptr ));

template <typename...>
false_type has_construct_f(...);

template <typename T, typename... Args>
struct has_construct: decltype(has_construct_f<T, Args...>(nullptr)) {};

struct A;
struct S {
    int construct(int, float);
    void construct(int, int, int);
    void construct(A&&); // declval должен уметь и такое обрабатывать
};
```
Важно сказать, что `A` -- неполный тип (incomplete type) -- тот, у которого нет определения. Работать с неполным типом можно только через &&.

## has_move_if_noexcept
```c++
template <typename T>
true_type has_move_if_noexcept_f(my2::enable_if_t<noexcept(T(my4::declval<T>())), decltype(nullptr)>);

template <typename...>
false_type has_move_if_noexcept_f(...);
```

## is_base_of
Самая е*анутая
```c++
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
```
Притом наличие хотя б одного аргумента в `is_base_of(int)` обязательно, ибо иначе обе функции равнозначны и получаем `ambiguous call`


## common_types
Общий тип из `Types...`. 

Вообще, у нас уже есть механизм, дающий нам общий тип для двух типов -- это тернарный оператор. От него и пляшем.

```c++
template <typename T, typename U>
struct common_type<T, U>: std::type_identity<
  decltype(true ? my4::declval<T>() : my4::declval<U>())
> {};
```

