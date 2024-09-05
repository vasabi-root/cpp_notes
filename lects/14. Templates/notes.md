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

template <> // специализация TU
void f(int a, int b) { // никогда не вызовется, если есть 
                       // перегрузка для TT
    cout << " 2\n";
}

// перегрузка
template <typename T = double> // работает и без double по умолчанию
void f(T a, T b=1.5) {
    cout << "3\n";
}

template <> // специализация TT
void f(int a, int b) { // b = int(1.5) по умолчанию!
    cout << b << " 4\n";
}

void f(int a, int b) { // перегрузка
    cout << "5\n";
}

int main() {
    f(0, 1); // 5
    f(1); //4
    // если закомментить f5
    f(9, 1); // 4

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

[Полезное](https://www.reddit.com/r/cpp/comments/1b7jkxd/why_cant_partial_specializations_be_friends/) про дружественные шаблонные классы себя

