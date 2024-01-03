# Dependent names problem
Проблема зависимости имён: X -- зависимое имя, если её смысл зависит от подстановки параметра шаблона T
```c++
template <typename T>
struct S{
    using X = T;
};

template <>
struct S<int>{
    static int X;
};

int a = 0;

template <typename T>
void f() {
    S<T>::X * a;
    // T == int:    X * a;
    // T == double: double *a;
}
```
Для разрешения неодноначности существует ключевое слово `typename`, которое говорит компилятору, что выражение означает `тип данных`:
```c++
typename S<T>::X *a;
```
Ещё одна проблема:
```c++
template <typename T>
struct SS {
    template <int N, int M>
    struct A {};
};

template <>
struct SS<int> {
    static int A;
};

template <typename T>
void g() {
    SS<T>::A<1, 2> a;
    // T == int: A < 1, 2 > a;
    // T != int: A<1, 2> a;
}
```
Тут недостаточно просто написать `typename`, ибо он не учитывает возможность шаблона в типе. Для этого требуется прямо перед именем шаблонной структуры указать `template`:
```c++
typename SS<T>::template A<1, 2> a;
```

ВСЁ ЭТО ЛЮТОЕ КОЛДУНСТВО, И ЗАПРЕЩЕНО ЗАКОНАМИ КОДСТАЙЛА И ЭТИКЕТА ОТНОШЕНИЙ МЕЖДУ ЛЮДЬМИ
# Basic type traits
Проблема: когда нам требуется различная реализация, если параметры шаблона одинаковы, и если они отличаются:
```c++
template <typename T, typename U>
void f() {
    // ...
    if ("U == T") {

    } else {
        
    }
}
```
Решение: `метафункции от типов`:
```c++
template <typename T, typename U>
struct is_same {
    static const bool value = false;
};

template <typename T>
struct is_same<T, T> {
    static const bool value = true;
};

template <typename T, typename U>
void f() {
    // ...
    if (is_same::value) {

    } else {

    }
}
```
Похожим образом можно убрать константность:
```c++
template <typename T>
struct remove_const {
    using type = T;
};

template <typename T>
struct remove_const<const T> {
    using type = T;
};
```
Всё, перечисленное выше, есть в `<type_traits>` (c++11).

Там есть и `remove_reference (&), _pointer (*), _extenet ([])`. Есть `decay`, который:
1. Превращает ф-ю в указатель
2. Превращает [] в указатель
3. Снимает константность

c++14: шаблонный псевдоним
```c++
template <typename T>
using remove_const_t = typename remove_const<T>::type;
```

с++17:
```c++
template <typename T, typename U>
const bool is_same_v = is_same<T, U>::value;
```

# Variadic templates
Переменное количество параметров шаблона c++11
```c++
template <typename... Args>
void print(const Args&... args) {

}
```
`typename... tail` -- обозначение `пакета` объектов с разными типами

`tail...` -- распаковка пакета (список через запятую)

Пакет можно не только распаковать, но и узнать количество объектов в нём: `sizeof...(tail)`

Для просмотра, что сгенерил компилятор: `Compiler Explorer`

# Правила выбора шаблонов
Сохраняются ли ссылки и константы при шаблонной подстановке в ф-ю?  
Нет! Компилятор сбрасывает `&, const &, const T` -- еслия явно не указать шаблонный параметр

