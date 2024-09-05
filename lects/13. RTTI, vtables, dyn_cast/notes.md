# Проблема виртуальных деструкторов
`Base *b = Derived();`

`delete b;`

Деструктор наследника не вызовется!!!
Нужно объявить деструктор Base `virtual` -- всегда!
Это работает и для абстрактных классов

# RTTI dynamic_cast

## Run Time Type Info

```c++
int x;
cin >> x;

Base b;
Derived d;

Base &rb = x ? b : d; // RTTI
rb.f();
```
Так же компилятор не в силах понять, приватна ли `f` или публична в CT -- только RT

## dynamic_cast
Приведение типов, успешность которого зависит от RT-состояния  
Применим целесообразен только для полиморфных типов

Возвращает `nullptr`, если приведение не удалось, и адрес -- в противном случае

Может кастовать и на другую ветку (в отличие от `static_cast`, который кастует вдоль лишь одной ветки)
```c++
Mother m = Mother();
try {
    dynamic_cast<Father&>(m);
} catch(...) { // Ошибка времени выполнения (Aborted)
    cout << "OOOps\n";
}
```
Существует оператор проверки типа `typeid()`, которая учитывает полиморфизм.
Можно даже узнать имя класса `typeid(m).name()`. 

# VTables
Каждая структура вначале содержит указатель на `vtabe`
```c++
struct Base {
    int b;

    virtual void f() {}
    void foo() {} // Не хранится в структуре!!! Компилятор на этапе компиляции делает 
                  // JUMP по нужному адресу
};

struct Derived: Base {
    int d;

    void f() override {}
    virtual void g() {}
};
```
Переходы:
```js
Base: [vptr] [b]
        |
       \|/
      [typeinfo_ptr] [&f1]

Derived: [vptr] [b] [d]
           |
          \|/
      [typeinfo_ptr] [&f2] [&g]
```
При вызове `b.f()`, где b -- ссылка на Derived::Base, происходит 3 лишних прыжка по указателям (по сравнению с вызовом foo()) :
1. Base
2. vptr
3. \>\> 8
4. f2

Это всё замедляет исполнение -- поэтому в плюсах virtual отключён по умолчанию (не как в Jave)

<!-- <image src="https://i.ytimg.com/vi/_aANg3_U9Q0/maxresdefault.jpg" alt="Описание картинки"> --> 

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
