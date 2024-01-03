 Управляющие конструкции:

Scopes:
- Namespace -- только объявления
- Scope начинается, когда открывается фигурная скобка
- Обращение к глобальной переменной из локального скоупа: ::x
- Qualified id:     std::cout
  Unqualified id:   cout

Основные типы и операции над ними:
- Базовые:
    - Целочисленные:
       (- char      1)
        - short     2 
        - int       4 usually
        - long      4
        - long long 8
        - 1'123'123 -- std14

        int32_t
        int64_t -- size_t
        int128_t

        unsigned ...
        uint...

        Integer promotion:
            (int) + (unsigned int) = (unsigned int)

    - Плавающая точка:
        - float         4
        - double        8
        - long double   16 
    
    - Символьный:
        - char (лучше писать unsigned char)
        - wchar16_t 

    - Логический:
        - bool (true, false, ||, &&, ==) 1B

- Контейнеры:
    - Строка
    - std::vector
        std::vector<int> v(<num>);
        v.resize(<num>)
        v.push_back();
        v.pop_back();
        v.front();
        v.back();
        v.empty(); // пустой ли?
    - std::queue
    - std::stack
    - std::set // упорядоченное мн-во 
    - std::unordered_set<type>
        .insert() // вставляет новый элемент (если элемент существует, игнор)
        .erase() // удалить
        .count(<elem>) // присутствует элемент в мн-ве или нет
        .size()
    - std::map<std::string, int> m; // py::dict
    - std::unordered_map<std::string, int> m; 

Управляющие конструкции:
    - Условия
    - Циклы

++i лучше, чем i++ потому, что не создаёт копию!!!
    