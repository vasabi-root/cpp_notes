# Templates
Логика первого порядка (кванторы)
```c++
template <typename T>
void func(const T& a, const T& b) {}
```
При этом область видимости -- следующая ф-я или след. класс после объявления шаблона.
Вызов `func(1, 1.0)` приведёт к CE ибо `T` у обоих параметров один

c++11: Шаблонные псевдонимы
```c++
template <typename T>
using eq_map = std::map<T, T>;
```

c++14: Шаблонные переменные
```c++
template <typename T>
const T pi = 3.14;

pi<double> == 3.14
pi<int> == 3
```

Есть ещё шаблонные методы:

```c++
template <typename T>
class Vec {
    template <typename U>
    void f(U a);
};

template <typename T>
template <typename U>
void Vec<T>::f(U a) {}
```
`typename` и `class` в данном контексте эквивалентны!

# Перегрузка шаблонных функций
Правила следующие: 
1. Если есть правило для общего и частного случаев, выбор падает именно на частный случай
2. Если получается получить точное соответствие, то выбор падает на эту версию

# Специализация шаблонов
"Для конкретного типа сделай другую реализацию, чем для остальных"
```c++
template <typename T>
class vec{

};

// Полная специализация
template <> 
class vec<bool> {

};

// Частичная специализация
template <typename T> 
class vec<T*> { // [], &, const, const &

};
```
Для функций частичная специализация запрещена стандартом, ибо у них есть `перегрузки` (в отличие от классов).

Полная специализация ф-й возможна. Но сама специализация определяется `после того, как была выбрана шаблонная версия ф-и`
```c++
template <typename T, typename U>
void f(T a, U b) {
    cout << "1\n";
}

template <>
void f(int a, int b) {
    cout << "2\n";
}

template <typename T>
void f(T a, T b) {
    cout << "3\n";
}
```
Можно определить тип в шаблоне по умолчанию: `template <typename T = int>`

# Non-type шаблоны
Классы с различными типами в шаблоне НЕПРИВОДИМЫ.

Параметрами шаблонов могут быть не только типы, но и `числа` (size_t, int, long long, char, bool). Это может пригодиться там, где операции над одним и тем же типом, но с разным количеством чего-либо, не должны работать (типо матрицы)

`matrix<2, 2>() + matrix<2, 3>();` CE!

Иначе (если бы мы передавали эти параметры в конструкторе) мы сможем их сложить.
НО: эти числа должны быть известны CT -- переменные нельзя.

Помимо этого, параметрами шаблона могут быть `другие шаблоны`
```c++
template <typename T, template<typename> class Container >
struct  Stack {
    Container<T> c;
};

Stack<int, std::vector>

// или 
template <typename T, typename Container = std::vector<T>>
struct  Stack {
    Container c;
};
```
