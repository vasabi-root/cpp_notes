# Порядок создания компонентов объекта
1. Поля и их инициализация родителя (при этом вызывается конструктор без параметров, если не указать:)
Dirived(): Base(4), param(2) {} 
2. Методы
3. Конструктор
4. 1-3 для наследника 

# Касты между родителем и наследником
Наследник должен быть устроен так, чтобы можно было поставить его вместо родителя и всё продолжит работать (но не обратно)  
Аналогия: `const int == Base, int == Derived: Base`

Если имеется ф-я требующая наследника в кач-ве параметра, а мы подсовываем родителя -- CE -- компилятор запрещает приведение типов от родителя к наследниику!!!  
// Но можно кастовать ЯВНО ссылки! Работает, только если до этого Base был проинициализирован Derived, иначе -- UB

`Derived& rd = static_cast<Derived&>(rb2);`  
`reinterpret_cast` -- опасен, не проверяет приватность

```c++

Derived d;
// slicing
Base b = d;   // вызывается конструктор копии Base
Base *b = &d; // колдунство -- получаем контроль над родительской частью 'd'
Base &b = d;  // колдунство -- получаем контроль над родительской частью 'd'
```

`private` // `protected`- наследник не может быть приведён к родителю!

Запрет каста от наследника к родителю (не средствами `private` // `protected`) НЕВОЗМОЖЕН!

# Множественное наследование
Проблемы:
1. В памяти: `[Father::f] [Mother::m] [Son::s]`
2. Наследование от классов с одинаковыми названиями полей

Тогда при обращении к этим полям имеем неоднозначный вызов (нужно явно указывать)
`s.Father::a`

```c++
struct Father {
    a = 1;
};

struct Mother {
    a = 1;
};

struct Son: public Father, public Mother {
    s = 3;
};
```

Наследование от 2х родителей от одного предка -- ромбовидное наследование  
Инцест -- это плохо))))

Тогда имеем 2 Granny -- одна от мамы, вторая -- от папы
Это разные объекты, но выбрать, к какому обращаться невозможно -- CE
Static cast -- тоже CE

Единственное:  
`Mother &m = static_cast<Mother&>(s);`  
`Granny &g = static_cast<Granny&>(m);`