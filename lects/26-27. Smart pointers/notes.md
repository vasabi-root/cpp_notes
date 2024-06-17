# RAII. Smart ptrs
Для:
- `new` `delete`
- `lock(mutex)` `unlock(mutex)`
- `open connection` `close connection`

## `unique_ptr`
Простейший. Раньше сложно было с ним работать, ибо его нельзя копировать. Но с [c++11] появилась мув-семантика. Теперь можно мувать вместо копирования.

## `shared_ptr`
Имеет счётчик ссылок на своё содержимое. Этот счётчик должен увеличиваться на 1 в конструкторе копии, и уменьшаться на 1 в деструкторе. Его нельзя делать статик, ибо так он будет работать вообще для всех объектов `shared_ptr`. Нам нужно что-то, что будет меняться бродкастом для всех `shared_ptr` с одним и тем же указателем. Это почти определение указателя -- именно он нам и нужен.

Но что делать со случаем:
```c++
int *p = new int(1);
shared_ptr sptr1(p);
shared_ptr sptr2(p); 
// дабл фри 🍟🍟
```
Нуууу. Вообще, можно было бы мувать (или просто занулять указатель, от которого мы отпочковались), но комитет почему-то решил, что лучше не решать эту проблему (так-то shared_ptr устроен куда сложнее, мб там какие-то веские причины есть, пока не знаю).

Есть ещё одна проблема: мы вызываем `new` 2 раза. Это нехорошо, ибо долго -- нужны аллокаторы и штука, которая бы скрыла от нас явное использование `new` при инициализации указателя. Эта штука -- функция `make_shared`. Но сначала, `make_unique` (интересно, что `make_shared` появился в c++11, а `make_unique` -- в c++14)
```c++
template <typename T, typename ...Args>
shared_ptr<T> make_unique(Args&&... args) {
    return shared_ptr<T>(new T(std::forward<Args>(args)...));
    // можно без forward_if_noexcept, ибо new не кидает исключений
    // точнее, если он кинет, то он обязуется освободить то, что навыделял
}
```
Теперь можно реализовать и `shared_ptr`. Сразу же решаем проблему двойного вызова `new` (правда, пока костыльно и непонятно, что с выравниванием)
```c++
template <typename T, typename ...Args>
shared_ptr<T> make_shared(Args&&... args) {
    void* p = ::operator new(sizeof(T) + sizeof(size_t)); // проблема выравнивания
    new (p) T(std::forward<Args>(args)...);
    new (static_cast<T*>(p)+1) size_t();
    return shared_ptr<T>(new T(std::forward<Args>(args)...));
}
```
Лучше обернуть `T` и `size_t` в структуру, и оперировать с ней -- без костылей и с выравниванием.

Есть ещё одна проблема -- циклические ссылки в `T`:
```c++
T *root = new T();
T *child = new T();
root->next = child;
child->prev = root;

auto root = make_shared<T>();
auto child = make_shared<T>();
root->next = child;
child->prev = root;
```
Для решения этой проблемы придумали `weak_ptr`. 
### `weak_ptr`
Просто наблюдает. Допускает уничтожение объекта.

Фишка в том, чтобы одну из "сторон" указателей сделать `weak_ptr` (например, все next -- это weak_ptr). Тогда нужно ещё счётчик вик_птр'ов. При этом вызов деструктора должен происходить в вик, а деаллокация -- в шэйрд.

## `enable_shared_from_this`
Иногда хочеца такого:
```c++
struct Node {
  Node *next;

  shared_ptr<Node> get_shared() {
    return this; // некорректно. Дабл фри 🍟🍟
  }
}
```
Решение супер-неочевидное: отнаследоваться от класса `enable_shared_from_this` и возвращать в таких случаях `shared_from_this`. `CRTP`
```c++
template <typename T>
struct Node: public std::enable_shared_from_this {
  Node *next;
  T object; 

  shared_ptr<Node<T>> get_shared() {
    return shared_from_this; // OK
  }
}
```
Доп проблемы по поводу аллокатора [тут](https://youtu.be/9ZSBOfTd-sc?t=3943)

[про 301 балл порога в вузах](https://youtu.be/xzK6Zx0TZx8?t=2790)