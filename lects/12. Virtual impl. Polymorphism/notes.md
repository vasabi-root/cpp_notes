# Виртуальное наследование
Решает проблему ромбовидного наследования
```c++
struct Granny {
    int g = 0;
};

struct Father: virtual public Granny {
    int f = 1;
};

struct Mother: virtual public Granny {
    int m = 2;
};

struct Son: public Father, public Mother {
    int s = 3;
};
```
Но! Теперь <sizeof(Son)> == 40, а не 20, как было до этого.  
```
Ибо раньше: [g] [f] [g] [m] [s] == 20
Теперь:     [f*G] [f] ... [m*G] [m] ... [s] [g] ... == 40
            0     8       16    24      28  32      40
```
**Обязательно надо, чтобы все классы с общим предком имели его именно в виртуальном виде**  
Если делать так:
```c++
struct Granny {
    int g = 0;
};

struct Father: public Granny {
    int f = 1;
};

struct Mother: public Granny {
    int m = 2;
};

struct Son: virtual public Father, virtual public Mother {
    int s = 3;
};
```
```
То: [*FS] [s] [g] [f] [g] [m]
     |
    \|/
    [*F] [*M]
```
Проблема осталась!!!

Притом приватность и публичность не влияют на результат неоднозначного обращения (ибо эта проверка делается в самом конце компиляции)

# Полиморфизм
Задача: расчитать площадь многоугольнка; расчитать площадь квадрата. Эти задачи мы будем решать по-разному, несмотря на то что квадрат -- это многоугольник.

Виртуальная функция -- это такая, что если обратиться к версии для наследника через ссылку на родителя, то вызовется версия *для наследника*

`virtual` понижает приоритет, притом изначально при обращении к функции через ссылку на родителя  -- приоритет выше у предка  
При этом сигнатура должна быть `абсолютно одинаковой` (включая константность)  
Но если функция виртуальна и не определена, то её нужно обязательно переопределить в потомке

Полиморфизм -- разные объекты могут совершать одни и те же операции по-разному  
Бывает `статический` (выбор в CompileTime) и `динамический` (в RunTime), но перегрузка всегда CompileTime

Тип называется `полиморфным`, если в нём есть хотя бы один виртуальный метод

**c++11**
`override` -- ключевое слово, при котором помечается, что функция в этом месте переопределяется (будет CE, если нет такой виртуальной функции у предка -- по кодстайлу надо писать override). Это слово существует для того, чтобы компилятор почаще ругался

`final` -- запрет переопределения виртуальной ф-и

# Абстрактные классы. Чисто виртуальные ф-и
Чисто виртуальная функция -- та, которую нельзя вызвать без переопределения (хотя её можно определить и вызывать явно в наследнике -- для нужд дебага)
```c++ 
virtual void f() = 0;
```
Если в классе есть хотя бы один виртуальный метод, то такой класс называется `абстрактным` -- нельзя создать экземпляры такого класса.

Но виртуальные функции не обязывают наследников перепределять их -- хотя в таком случае наследник будет считаться абстрактным и его экземпляр будет нельзя создать.

Если же объявить виртуальную функцию и создать наследника без определения, то получим ошибку линкера -- в VTable надо положить адрес функции, а его нема

В конструкторе механизм виртуальных функций отключается!!!!!!! (ошибка линковки)

Но при этом, если обернуть вызов виртуальной функции в обычную, то получим `pure virtual method called` -- ибо на самом деле, чисто виртуальная функция -- это не nullptr -- это указатель на функцию, которая выводит сообщение об ошибке и кидает `terminate`  
Почему? Ибо этап создания объекта дочернего класса таков: 
```
родительские поля и методы -> 
инициализация `vpointer` таблицей родителя -> 
инициализация полей родителя -> 
конструктор родителя -> ... -> 
переприсвоение `vpointer` на дочернюю инициализацию.  
```
То есть во время вызова конструктора, vpointer указывает на таблицу родителя!  
При этом в деструкторе механизм виртуальных фукнций не отключается

Важно понимать, что подстановка аргументов в функцию происходит в CT, поэтому могут быть нежданчики при использовании различных значений по усмолчанию для аргументов различных реализаций функций

# Виртуальный деструктор