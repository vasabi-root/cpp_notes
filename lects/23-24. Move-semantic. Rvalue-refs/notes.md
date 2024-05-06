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
## std::move()
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
```c++
// copy-construct
vector(const vector& s);
// copy-assignment op
vector& operator= (const vector& s);

// move-construct
// TODO: WTF vector&&??
vector(vector&& vec) noexcept(noexcept(alloc(std::move(vec.alloc))))
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
    arr = vec.arr;
    sz = vec.sz;
    cap = vec.cap;
    alloc = std::move(vec.alloc);

    // так же очистить vec
}
```
- для чисел и указателей нет смысла мувать!  схема как при взятии ссылки -- легче копировать, чем мувать