# Вывод типов
## auto
`auto` и `dectype` [c++11]

Как с шаблонами:
- `auto`: по значению
- `auto &`: по lvalue-ссылке
- `const auto`: по константной ссылке
- `auto &&`: по универсальная ссылка
- `const auto &&`: по const rvalue ссылке

Полезности:
```c++
auto &rx = 5; // CE
const auto &crx = 5; // OK: часто  используется в проде
auto &&rrx = 5; // OK
auto &&rrx2 = crx; // OK: универсальная ссылка сократится до int&
                   // неконстантная lvalue ссылка
const auto &&crrx = crx; // CE
```

Когда `auto` неуместно:
- Тип функций (можно, но не нужно). Если есть ветвления `return` с разными типами, то будет вообще CE (хотя можно привести к `?:`). При этом, компилятор СНАЧАЛА выводит тип, а ПОТОМ делает оптимизации (включая RVO)
- Обращение по индексу к vector<bool>. Авто определит `BitReference&`, а не `bool` -- так что при изменении новой переменной, будет изменятся и соответствующий элемент вектора.\

По функциям. Начиная с c++20 появилась возможность писать auto для параметров функции. Но это просто шорткат для шаблона.
```c++
template <typename T, typename U>
T f(const T& a, const U& b) {
  return a + b;
}

auto f(const auto& a, const auto& b) {
  return a + b;
}
```

## decltype
Позволяет в CT получить тип выражения. Отличие от `auto` -- `decltype` не отбрасывает амперсанды. Но можно применять модификаторы типа `*, &, const`. Важно: деклтайп не вычисляет выражение внутри своих скобок, а лишь определяет его тип!!!
```c++
const decltype(throw 1)* pvoid; // throw 1 -- это выражение, которое ничего 
                                // не возвращает. Имеем [throw 1] == void. 
                                // Итого, [p] == const void*
```
Работа decltype зависит от того, что он анализирует:
- Если это просто переменная (без скобочек -- unparenthesized), то результатом будет просто тип переменной
- Если это выражение, то
  - если `lvalue`, то будет добавлен `&`
  - если `prvalue`, то ничего не довешивается
  - если `xvalue`, то будет добавлен `&&`
- Если в скобочках, то будет как с lvalue

Можно писать `decltype` для типа выражений. Это может потребоваться при работе с контейнерами, например:
```c++
template <typename Container>
auto& getByIdx(Container &cont, size_t idx) { // вектор булей не должен возвращать 
                                              // ссылку, но в этой реализации он именно 
                                              // её и вернёт
  return cont[idx];
}
```
Здесь нужен `decltype`, который не отбрасывает ссылку, но ему нужно передать аргументы. Как?
```c++
template <typename Container>
auto getByIdx(Container &cont, size_t idx) 
  -> decltype(cont[idx]){  // если бы мы расположили это вместо авто, то получили бы CE
                           // ибо cont и idx ещё не обявлены
  return cont[idx];
}
```
Но тут придётся менять значение в скобках, если меняется ретёрн -- поэтому есть более удобная форма [c++14]:
```c++
template <typename Container>
decltype(auto) getByIdx(Container &cont, size_t idx) {
  return cont[idx];
}
```
На decltype(auto) нельзя вешать `&, *, const`

## typeid. Class template argument deduction CTAD 
[c++17] Можно не писать явно шаблонные аргументы при создании экземпляра класса, если компилятору очевиден тип. Когда не очевидно?
```c++
vector v(5, 0); // OK
vector vv{v.begin(), v.end()};  // вектор итераторов на вектор
vector vv(v.begin(), v.end());  // вектор интов
```
Можно явно указать, как должен работать CTAD: user-defined deduction guides
```c++
template<typename T>
class C {
  C() = default;

  template<typename Iter>
  C(Iter b, Iter e) {}
}

template <typename Iter>
C(Iter b, Iter e) -> C<typename std::iterator_traits<Iter>::value_type>;

C c1(v.begin(), v.end()); // OK
C c2{1, 2};               // CE: int не итератор -- у него нет поля value_type
```
Когда использовать? Маст-хэв в `std::pair` и `st d::tuple`

## Structure bindings
Как в питоне
```c++
std::unordered_map<int, string> map;
for (auto& [key, value]: map) {
  ...
}
```


## typeid
Определяет тип в RT