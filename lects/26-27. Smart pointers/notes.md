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
Лучше обернуть `T` и `size_t` в структуру, и оперировать с ней -- без костылей и с выравниванием. Но тогда придётся реализовывать различные сценарии при формировании ш_птр от указателя и от цб. Ибо память будет аллоцирована по-разному. Доп проблемы по поводу аллокатора [тут](https://youtu.be/9ZSBOfTd-sc?t=3943)

Также есть смысл делать наследование в виде:
```c++
struct BaseControlBlock {
  size_t shared_count;
  size_t weak_count;
};

template <typename Deleter, typename Alloc>
struct ControlBlock {
  Deleter del;
  Alloc alloc;
}

template <typename T, typename Alloc> // Deleter не нужен, раз мы сами вызываем new
struct ControlBlockMakeShared: BaseControlBlock {
  T object;
  Alloc alloc;
};

```

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

Фишка в том, чтобы одну из "сторон" указателей сделать `weak_ptr` (например, все next -- это weak_ptr). Тогда нужно ещё счётчик вик_птр'ов. При этом вызов деструктора должен происходить в вик, а деаллокация -- в шэйрд. Когда `вик_каунтер` опустился до нуля, надо вызвать деструктор, а когда обнулился `шейрд_каунтер` -- деаллоцировать.

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
    return shared_from_this(); // OK
  }
}
```
Как реализовать `enable_shared_from_this`? 
```c++
template <typename T>
struct enable_shared_from_this {
  weak_ptr<T> wptr__;

  shared_ptr<T> shared_from_this() {
    if (wptr__.ptr == nullptr)
      throw 1;
    return wptr__.lock();
  }
}

template <typename T>
class shared_ptr {
  ...
public:
  shared_ptr(T* ptr): ptr_(ptr), count_(new size_t) {
    *count_ = 1;
    if constexpr (std::is_base_of_v<enable_shared_from_this<T>, T>) {
      wptr__ = *this;
    }
  }
};
```
Дальше больше -- shared_ptr не имеет полей аллокатора и делитера!!! А ещё, как присваивать два ш_птр с разными аллокаторами/делитерами?

# Type erasure
Язык C++ статически типизирован. Правда?
```c++
#include <any>
#include <vector>

int main {
  std::any a;
  a = 0;
  a = 0.3;
  a = "sdfsdf";
  a = std::vector<int>(3);
}
```
И всё же да, ведь если мы захотим обратиться к хранимому элементу, то придётся делать каст:
```c++
  a = 0.5;
  cout << std::any_cast<double&>(a) << endl;
```
Как такое реализовать? Нам нужно нечто, что будет менять своё поведение в рантайме. Это похоже на виртуальные функции, но только относительно полей:
```c++
struct any {
  struct Base {
    virtual Base* getCopy() const {}
    virtual ~Base() {} // необязательно, но раз есть одна виртуальная функция
                       // то лучше всё же сделать деструктор виртуальным
  };

  template <typename T>
  struct Derived: Base {
    T object;
    Derived(const T& object): object(object) {}
    Derived(T&& object): object(std::move(object)) {}
    Base* getCopy() const override { return new Derived(object); }
  }

  Base* ptr;

  template<typename T>
  any(const T& obj): ptr(new Derived(obj)) {}
  any(const any& other): ptr(other.getCopy())  {}
  ~any() { delete ptr; }

  template <typename T>
  friend T any_cast(const &any a) {
    auto d = dynamic_cast<Derived<T>*>(a.ptr);
    if (!d) throw 1;
    return d.object;
  }
};



```



[про 301 балл порога в вузах](https://youtu.be/xzK6Zx0TZx8?t=2790)