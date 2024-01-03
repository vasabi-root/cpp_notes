# 8. Контейнеры
## 8.1. Overview
CPP-референс -- какие типы могут называться контейнерами  
1. `Sequence-контейнетры`  
Хранение последовательности объектов (`vector`, `array`, `deque`, `forward_list`, `list`)  
`stack` и `queue` -- не контейнеры, а адаптеры над контейнерами
2. `Associative-котейнеры`  
Хранение значений по ключам (`map`, `set`, `unordered_map`, `unordered_set`)  
set -- по ключу хранится bool (есть элемент или нет)

<span style='color:lightgreen'>O(1)</span>  
<span style='color:yellow'>O(log(n))</span>  
<span style='color:red'>O(n)</span>  

Container | [ ] | push/pop_back | push/pop_front | find | insert | erase | iterators | reference invalidation | iterator invalidation
--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
`vector` | <span style='color:lightgreen'>O(1)</span> | <span style='color:lightgreen'>O(1)</span> amort | - | <span style='color:red'>O(n)</span> | <span style='color:red'>O(n)</span> | <span style='color:red'>O(n)</span> | random access | <span style='color:red'>YES | <span style='color:red'>YES
`deque` | <span style='color:lightgreen'>O(1)</span> | <span style='color:lightgreen'>O(1)</span> amort | <span style='color:lightgreen'>O(1)</span> amort | <span style='color:red'>O(n)</span> | <span style='color:red'>O(n)</span> | <span style='color:red'>O(n)</span> | random access | <span style='color:lightgreen'>NO | <span style='color:red'>YES
`list` / `forward_list` | - | <span style='color:lightgreen'>O(1)</span> | <span style='color:lightgreen'>O(1)</span>| <span style='color:red'>O(n)</span> | <span style='color:lightgreen'>O(1)</span> | <span style='color:lightgreen'>O(1)</span> | bidir/forward | <span style='color:lightgreen'>NO | <span style='color:lightgreen'>NO
`set` / `map` | map: <span style='color:yellow'>O(log(n))</span> | - | - | <span style='color:yellow'>O(log(n))</span> | <span style='color:yellow'>O(log(n))</span> | <span style='color:yellow'>O(log(n))</span> | bidir
`unordered_set` / `unordered_map` | <span style='color:lightgreen'>O(1)</span> expected | - | - | | <span style='color:lightgreen'>O(1)</span> expected | <span style='color:lightgreen'>O(1)</span> expected | forward


amort -- потратив N запросов имеем в среднем такую-то сложность
expeсted -- вероятность при обычном выполнении запроса (если хэш-функция подобрана криво под данные, то можем иметь O(N) каждый раз)

`deque` -- как вектор, только можно добавлять элементы в начало И элементы хранятся непоследовательно: используются связанные мини-батчи (2 перехода по указателю против одного в векторе)  
`set/map` -- R/B-tree  
`unordered_map|set` -- хэш-таблица  

## 8.2. Iterators
```
Input Iterator: [pass once], {ifstream}
|.
Forward Iterator: [pass cycle], {forward_list}
|.
Bidirectional Iterator: [pass bidir, --], {list}
|.
Random Access Iterator: [+=n, -=n, >, <, >=, <=, it1-it2], {deque}
|.
Contigious Iterator (c++17): [&(*it2) == &(*it1)+1], {vector, array, T*}
```
Итераторы -- не классы, не типы!  
Тип называется итератором, если удовлетворяет некоторым условиям  

У итератора должен быть определён `using value_type = ...;` -- тип под итератором  
У итератора должен быть определён `using iterator_categogry = ...;` -- так можно отличить вид итератора  (сделать is_base_of с `std::..._itarator_tag`)

```c++
template <typename Iter> // шаблонный параметр называются по виду итератора
void process(Iter begin, Iter end) {
    --end; // [begin, end)
    typename Iter::value_type tmp = *begin;
    *begin = *end;
    *end = tmp;
    if (
        std::is_base_of<typename Iter::iterator_category, 
                        std::random_access_iterator_tag>      // Compile time
    ) {} 
}
```

`std::iterator_traits` -- структура, цель которой -- доопределить юзинги итератора, если они не определены (можно даже сишный поинтер доопределить):
```c++
std::is_base_of<typename std::iterator_traits<Iter>::iterator_category, 
                std::random_access_iterator_tag>
```

## 8.3. vector
Увеличивается в 2 раза, когда достигает некоторой ёмкости

После ресайза нельзя сразу присваивать значение по последнему адресу -- память зарезервирована, но не проинициализирована (имеем сырые байты). Чтобы проинициализировать уже выделенную память, используется синтаксис (размещающий new):
```c++
new (<указатель>) T(<значение>) // placement new
```

Вызов деструктора без удаления памяти (надо оставить сырую память):
```c++
(<указатель>) -> ~T 
```

**Exceptions safety**  
1. `new`: bad_alloc -- кидаем наверх, так как он ничего не меняет (т.е. не нарушает ES)
2. `T()`: броски в конструкторе -- нарушауют ES, `необходимо обработать`

После ресайза/резёрва, следующие структуры в векторе становятся нерелевантными, если были проинициализированы до ресайза/резёрва:
- Указатель/ссылка на вектор (UB)
- Итератор (см реализацию разыменования итератора) (UB)

Кста у `deque` обе проблемы с инвалидацией отсутствуют

### 8.4. vector < bool >
Хочется, чтобы под 1 буль выделялся 1 бит. Но что тогда возвращает обращение по индексу?
```c++
std::vector<bool> vb(5, false);
vb[2] = true;
cout << typeid(vb[0]).name() << endl; // BitReference
```
BitReference -- пример многих исключений из правил:
- обращение по индексу возвращает копию, а не ссылку (но присвоение rvalue всё равно работает)
- *it возвращает не `bool`, а `BitReference`