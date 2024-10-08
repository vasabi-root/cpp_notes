# Move-семантика
since c++11. 
Позволяет избавиться от лишних копий.

## Мотивация
```c++
template <typename T>
void swap(T& a, T& b) {
    T c = a; // копия!!! А ведь можно просто подвигать биты... 
             // Но не при текущем уровне синтаксиса!
    a = b;
    b = c;
}
```
Или например. Сколько раз вызовется конструктор стринга?
```c++

int main() {
    std::vector<std::string> v;
    v.push_back("str"); // 1 -- временный объект строки
                        // 2 -- вызов конструктора копии в 'placement new'
    return 0;
}
```
Самый смак происходит в `reserve`. Там ведь удаляется целый массив, а затем создаётся массив большего размера, но элементы те же... Снова копии  
Можно проще) При реализации `оператора +`:
```c++
S S::operator+ (S a, const S& b) { // a -- копия
    a += b
    return a; // копия от 'a'. А так хочется просто создать поля объекта, не вызывая
              // конструктор, а затем просто сделать пересадку (кражу) полей из 'a' в 'temp'. 
              // Но чтобы работало за O(1), как будто бы надо пересаживать не поля,
              // а заголовки объектов
}
```
## `std::move()`
### Магия
Магическая функция. Совершает кражу бит. 

Правило такое -- вместо копирования на новое место, мы муваем на новое место
```c++
template <typename T>
void swap(T& a, T& b) {
    T c = std::move(a); // a теперь пуста!
    a = std::move(b);   // b теперь пуста!
    b = std::move(c);   // c теперь пуста!
}
```
Начиная с c++11, `vector::emplace_back()` работает быстрее. Если обычный `push_back()` делает 2 копии, то `emplace_back()` принимает на вход **параметры** конструктора объекта и делает копию лишь один раз

Но `move`-семантику поддерживают только правильно реализованные типы! Это те, у которых реализован `move-constructor` и `move-assignment operator` :

- для чисел и указателей нет смысла мувать!  схема как при взятии ссылки -- легче копировать, чем мувать
- `clear()` для зануления вызывать нельзя, ибо он не делает `delete []`. Но зачем чистить, если мы забрали себе этот адрес через arr(vec.arr)?...

```c++
// copy-construct
vector(const vector& s);
// copy-assignment op
vector& operator= (const vector& s);

// move-construct
// TODO: WTF vector&&??
vector(vector&& vec) noexcept(alloc(std::move(vec.alloc)))
    : arr(vec.arr) 
    , sz(vec.sz), cap(vec.cap) // для чисел и указателей нет смысла мувать! 
                               // схема как при взятии ссылки -- легче копировать, 
                               // чем мувать
    , alloc(std::move(vec.alloc))  // если бы было alloc(alloc), была бы копия!
{
    // vec.clear() -- нельзя, он не делает delete []
    // TODO: vec.arr -- через AllocTraits 
    vec.arr = nullptr;
    vec.sz = 0;
    vec.cap = 0;
    // vec.alloc -- уже занулён, так как для него вызвана std::move
}

// move-ass op
vector& vector(vector&& vec) noexcept(noexcept(alloc(std::move(vec.alloc)))) {
    // TODO: arr -- через AllocTraits 
    for (el : arr) {
        AllocTraits::destroy(alloc, el);
    }
    AllocTraits::deallocate(alloc, arr);

    arr = vec.arr;
    sz = vec.sz;
    cap = vec.cap;
    alloc = std::move(vec.alloc);

    // так же очистить vec
    for (el : vec.arr) {
        AllocTraits::destroy(alloc, el);
    }
    AllocTraits::deallocate(alloc, vec.arr);
}
```


Теперь имеем не правило 3 (когда надо реализовать конструктор копии, присваивание и пользовательский деструктор), а правило 5... c++11. Если реализовать только первые 3, то будет медленно работать.

Компилятор генерирует мув-конструктор и оператор по умолчанию. Тогда он просто вызовет std::move() для каждого поля. Но для ссылок и указателей он не сделает отчистку. Но! Если реализован кастомный конструктор копии или оп=, то он не будет генерить мувы! И обратно: если реализованы кастомные мувы, то копи-контруктор и оп= не будут сгенерены (но можно сделать `S(const S&) = default`).

### Реализация
std::move() -- это просто странный каст. Поэтому он не разрушает объект (можно сделать std::move(x); и ничего не будет)
```c++
template <typename T>
??? move(??? x) { 
    return static_cast<???>(x);
}

template <typename T>
std::remove_reference_t<T>&& move(T&& x) {  // Должно же быть CE при попытке передать lvr...
// но тут всё сложнее -- позже
    return static_cast<std::remove_reference_t<T>&&>(x);
}
```



## Rvalue Lvalue
Раньше мы считали, что они отличаются друг от друга только возможностью присваивания. Но всё же `vector<bool> + vector<bool>`  присваивать можно!

Так воот. Теперь самое главное об `rvalue` и `lvalue`:
1. Эти термины относятся не к состоянию объекта, а к состоянию ВЫРАЖЕНИЯ (expression)
2. `expression` -- это синтаксическая конструкция из переменных, констант, операторов. Оканчивается точкой с запятой.
3. Можно написать такие выражения с одним и тем же объектом, некоторые из которых будут `rvalue`, а некоторые -- `lvalue`.

`NUD` -- not user-defined
| # | `Lvalue` | `Rvalue` |
|:---:|:---|:---|
| `1` | идентификаторы (имя переменной) | литерал (константа времени компиляции) |
| 2 | NUD операторы присваивания `= += -= *= /= &= ...`  | NUD операторы `+ - * / % & << >> > <...` |
| 3 | prefix++ | postfix++ |
| 4 | унарная * | унарные `& ~ !` |
| 5 | op `?:` когда оба операнда `lvalue` | op `?:` когда хотя бы один операнд `rvalue` |
| 6 | op `,` когда последний операнд `lvalue` | op `,` когда последний операнд `rvalue` |
| `7` | function call if return is `T&` | function call if return is `T` or `T&&` |
| `8` | cast to `T&` | cast to `T` or `T&&` |

То есть `&&` -- это сиснтаксис для того, чтобы ссылка была `rvalue`!!! Однако это не означает, что выражение `value;` будет `rvalue`, даже если у него тип `T&&` -- идентификатор всегда `lvalue`. Так мы помечаем объект, что возврат такого объекта должен быть `rvalue`

Проблема в том, что `T&&` не всегда означает rvalue-ссылку. Если это ссылка от `T&`, то амперсанды схлопнутся до одного.

Правила схлопывание ссылок (reference-collapsing):
- `&   &  -> &`
- `&   && -> &`
- `&&  &  -> &`
- `&&  && -> &&`
Идея в том, что если в цепочке вызовов объект хоть раз передали как lvalue-ссылку, то это свойство должно сохраняться. Иначе мы мувнем то, что мувать нельзя (то есть, украдём то, что красть нельзя).

Мув -- это оператор донорства. При этом `std::move()` -- метка донорства, которая быстро сгорает: срок хранения органов очень мал -- действует всего одно присвоение. Так что если метка сгорает сразу после того, как мы поставили `;`

Важно, что константная ссылка -- это не rvalue ссылка!!! Мы же создавали rvalue-ref, чтобы можно было похитить у неё данные, но если ссылка константная, то модифицировать объект нельзя! Ещё интересность:
- `lval-ref`: нужно инициализировать `lvalue`
- `rval-ref`: нужно инициализировать `rvalue`

## Примерчик
```c++
int x = 5;
int& r = x; // OK
int&& rr = x; // CE: попытка инициализировать rval через lval
int&& rr = std::move(x); // OK: x -- умер
int&& a = 5; // OK 
int& b = rr; // OK
int&& c = rr; // CE: выр-е rr здесь lval (rval выр-е -- это вызов функции)

const int& clvr1 = r; // OK
const int& clvr2 = rr; // OK
const int& clvr3 = std::move(rr); // OK: но константность останется с нами
int&& rvr = std::move(clvr1);// CE: хотим забрать органы того, кто запретил вмешательство в организм (same для lvr2 lvr3)

int& rvr = std::move(r); // CE: lval = rval;
int &rvr = 5;
const int&& crvr = std::move(rr); // OK: но бессмысленно

void f(int&& a) {}
void f(const int& a) {}

f(rr); // вызовется версия с const int&!!!! Ибо тут играет роль, какой тип выражения (lv rv), а не тип данных (метка всё ещё быстро сгорает)
f(std::move(rr)); // вот тут вызовется int&&
```
## Квалификаторы классов
По аналогии с тем, что существуют квалификаторы константных/неконстантных методов для константных/неконстантных объектов класса. 

Отличие в том, что если мы написали квалификатор &&, то отсутствие квалификатора не означает & (компилироваться не будет)! Отсутствие квалификатора -- это типа общая реализация

```c++
struct S {
    void f() & {          // для lvl объекта
        cout << "lvl\n";
    }

    void f() && {
        cout << "rvl\n";  // для rvl объекта
    }

    void f() const && {
        cout << "rvl\n";  // для const rvl объекта
    }
};
```
## Perfect-forwarding problem

Рассмотрим `emplace_back`:
```c++
template <typename... Args>
void emplace_back(const Args&... args) { // а что если некоторые rvl, а некоторые -- lvl??
    if (sz >= cap) {
        reserve(2*cap);
    }

    AllocTraits::construct(alloc, arr+sz, args...);
    ++sz;
}
```
Мы не можем просто взять и сделать `std::move(args)...`, ибо так можно мувнуть то, что мувать было нельзя.

Решение: `std::forward()`: если принимает rvl, то и передаст rvl (так же с lvl)
```c++
    AllocTraits::construct(alloc, arr+sz, std::forward<Args>(args)...);
```
То есть, `std::forward()` -- условный каст в rvlr
Как это реализовать?  
`[Crunch]` Для функций T&& -- это универсальная ссылка.  Теперь типы у следующих выражений такие `¯\_( ͡° ͜ʖ ͡°)_/¯` :
```c++
int x;                    // int&
int &lrx = x;             // int&
int &&rrx = std::move(x); // int
            5;            // int& 
```
Теперь по правилу сворачивания ссылок, можно реализовать `forward` (ему нужно явно передавать типы аргументов в шаблон):
```c++
template <typename T>
T&& forward_(std::remove_reference_t<T>& x) { 
    return static_cast<T&&>(x); // здесь T&& -- это универсальная ссылка, 
                                // которая может сворачиваться, НО!
                                // она такая только для шаблонных аргов функций (не классов)!!!
}

template <typename T>
T&& forward_(std::remove_reference_t<T>&& x) { 
    return static_cast<T&&>(x);
}
```


# Exception-safety move 
В `vector::reserve` мы писали что-то типа:
```c++
AllocTraits::construct(alloc, newarr+i, std::move(arr[i]));
```
Но это не `exception safe`. Существует безопасный относительно исключений мув:
```c++
AllocTraits::construct(alloc, newarr+i, std::move_if_noexcept(arr[i]));
```
Но этот мув работает только с типами, у которых конструктор `noexcept`!!!
Поэтому так важно писать `noxcept` в конструкторе (и в муве), если он не бросает исключений

# Проблема
Как обработать следующую ситуацию?
```c++
std::vector<int> vec(16);
vec.pushback(vec[1]);
```
То есть, мы хотим скопировать существующий элемент вектора и положить эту копию в конец. Проблема здесь в том, что если сначала мувнуть, а потом обратиться к `vec[5]`, то будет UB!! Ведь мы уже забрали органы у старого вектора. Как быть?  
`[Crunch]` Сначала перемещаем vec[5] на новое место, а затем муваем `◑﹏◐`

# По поводу чистого кода
чтобы быть объективнее, я посмотрел в дополнение [предмет обсуждения](https://www.youtube.com/watch?v=tD5NrevFtbU&t=0s)

Да, я согласен, что чистый код ухудшает перформанс. Но это как будто бы, это справедливо только для языков, которые работают с железом непосредственно. Но когда мы работаем с яву, там уровень абстракции уже настолько велик, что плевать, сделал ты 50 прыжков по виртуальной таблице, или 60 -- разницей можно пренебречь.

Но даже в плюсах есть места, где все эти штуки нужны. Тот же смарт поинтер нужен в первую очередь, чтобы программист меньше косячил. Да, он в конце говорит, что "работа программиста заключается в реализации производительной программы", но у него там игрушечный пример, когда кода больше 10k строк, ты просто не уследишь за всеми new delete, если хочешь, чтобы твоя программа не крашилась.