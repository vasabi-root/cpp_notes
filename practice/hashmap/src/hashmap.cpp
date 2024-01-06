#include <iostream>
#include <unordered_map>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <typeinfo>

using std::cout;
using std::cin;
using std::endl;

using std::unordered_map;

template <
    typename Key,                           // ключ
    typename Value,                         // значение 
    typename Hash = std::hash<Key>,         // хэш-функция
    typename KeyEqual = std::equal_to<Key>  // функтор эквивалентности
>
class Hashmap {
private:
    using value_type = std::pair<const Key, Value>;

    struct Node;
    struct List;

    Node **array_;

    size_t capacity_;
    double load_factor_;

    Node* insert_( Node *place, const Key &key, const Value &val=Value());
public:
    List buckets_;
    Hashmap(size_t capacity=256, double load_factor=1.0)
    : 
    capacity_(0),
    load_factor_(load_factor) 
    { reserve(capacity); }

    ~Hashmap() {
        buckets_.~List();
        delete [] array_;
    }

    Hash hashFunc;
    KeyEqual equal_to;

    struct iterator;

    iterator begin() { return iterator(buckets_.head->next); }
    iterator end() { return iterator(buckets_.head); }

    size_t get_hash(const Key &key) { return hashFunc(key) % capacity_; }
    double getLoadFactor(size_t cap) { return static_cast<double>(buckets_.sz) / cap; }

    Value& operator [] (const Key &key);

    void rehash();

    void reserve(const size_t &count) { // count -- количество элементов, а не корзин
        size_t cap = std::ceil(count / load_factor_);
        if (getLoadFactor(cap) < load_factor_) {
            if (capacity_ > 0) {
                delete [] array_;
            }
            array_ = new Node*[cap];
            for (size_t i = 0; i < cap; ++i) {
                array_[i] = nullptr;
            }

            capacity_ = count;
            rehash();
        }
    }

    void printBuckets() {
        Node *node = buckets_.head->next; 
        size_t i = 0;

        while (node != buckets_.head) {
            cout << node->hash << ": [" << node->kv.first << " " << node->kv.second << "]" << endl; 
            node = node->next;
            ++i;
        }

        cout << "array: ";
        for (size_t i = 0; i < capacity_; ++i) {
            if (array_[i] != nullptr) {
                cout << i << " ";
            }
        }
        cout << endl << endl;
    }
    
};


template <typename Key, typename Value, typename Hash, typename KeyEqual>
struct Hashmap<Key, Value, Hash, KeyEqual>::Node {
    Node *prev;
    Node *next;
    value_type kv;
    size_t hash;

    Node (value_type kv=value_type(Key(), Value()), const size_t &hash=0)
    : 
    prev(this), next(this),
    kv(kv), hash(hash) 
    {}
};

template <typename Key, typename Value, typename Hash, typename KeyEqual>
struct Hashmap<Key, Value, Hash, KeyEqual>::iterator {        
private:
    Node *node;
public:
    // iterator tags
    using value_type = value_type;
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using pointer           = value_type*;
    using reference         = value_type&;

    iterator(Node *n) : node(n) {};

    reference operator * () const { return static_cast<Node*>(node)->kv; }
    // pointer* operator -> () const { return static_cast<Node*>(node)->kv; }
    iterator& operator ++ () { node = node->next; return *this; }
    iterator& operator ++ (int) { iterator tmp = *this; ++(*this); return *tmp; }

    friend bool operator == (const iterator &it1, const iterator &it2) { return it1.node == it2.node; }
    friend bool operator != (const iterator &it1, const iterator &it2) { return it1.node != it2.node; }
};

template <typename Key, typename Value, typename Hash, typename KeyEqual>
struct Hashmap<Key, Value, Hash, KeyEqual>::List {
    Node *head;
    size_t sz;

    List() : head(new Node), sz(0) {}
    ~List() { retract_all(); }

    void push_front(Node *node) { insert(head->next, node); }
    void push_back(Node *node) { insert(head, node); }

    Node* erase(Node *node) { // требуется, чтобы node был в списке
        Node *prev = node->prev;
        Node *next = node->next;

        prev->next = next;
        next->prev = prev;

        // printBuckets();
        --sz;

        // delete node;
        return node;
    }

    void insert(Node *place, Node *node) {
        Node *prev = place->prev;

        // cout << "push_back" << endl;

        prev->next = node;

        node->prev = prev;
        node->next = place;

        place->prev = node;

        // printBuckets();
        ++sz;
    }

    void retract_all() {
        while (head->next != head) {
            delete erase(head->next);
        }
        delete head;
    }

    void printBuckets() {
        Node *node = head->next; 
        size_t i = 0;
        cout << head->hash << " -> ";
        while (node != head) {
            // cout << i << ": [" << node->kv.first << " " << node->kv.second << "][" << node->hash << "]" << endl; 
            cout << node->kv.first << " -> ";
            node = node->next;
            ++i;
        }
        cout << head->hash << endl;
    }
};

template <typename Key, typename Value, typename Hash, typename KeyEqual>
typename Hashmap<Key, Value, Hash, KeyEqual>::Node* Hashmap<Key, Value, Hash, KeyEqual>::insert_(Node *place, const Key &key, const Value &val) {
    Node *node = new Node({key, val}, get_hash(key));
    buckets_.insert(place, node);
    if (getLoadFactor(capacity_) > load_factor_) {
        cout << "REHASH" << endl;
        reserve(capacity_*2);
    }
    return node;
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
Value& Hashmap<Key, Value, Hash, KeyEqual>::operator [] (const Key &key) {
    size_t hash = get_hash(key);
    Node *place = array_[hash];

    if (place != nullptr) {
        while (
            place->next != buckets_.head &&     // пока не дошли до конца
            place->hash == hash &&              // хэш соответствует посчитанному
            !equal_to(place->kv.first, key)     // и ключ не совпадает с нужным
        ) {
            place = place->next;
        }

        if (
            // есди мы дошли до конца и ключ всё ещё не совпадает
            (place->next == buckets_.head && !equal_to(place->kv.first, key)) 
            // или мы уже дошли до конца корзины, то вершины нет -- надо создать новую
            || place->hash != hash
        ) {
            place = insert_(place, key);
        } 
    } else {
        place = insert_(buckets_.head, key);
        array_[hash] = place;
        // printBuckets();
    }
    return place->kv.second;
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
void Hashmap<Key, Value, Hash, KeyEqual>::rehash() {
    Node *curr = buckets_.head->next;
    List buckets_new;

    while (curr != buckets_.head) {
        size_t hash = get_hash(curr->kv.first);
        Node *next = curr->next;

        curr->hash = hash;

        if (array_[hash] != nullptr && array_[hash] != curr) {
            curr = buckets_.erase(curr);
            buckets_.insert(array_[hash], curr);
        } else if (array_[hash] != curr){
            array_[hash] = curr;
        } 
        curr = next;
    }
}

// using pair = std::pair<std::string, size_t>;

template <typename T>
struct hash {
    size_t operator () (const T &key){
        return *reinterpret_cast<const size_t*>(&key);
    }
};

template <typename T>
struct equal {
    bool operator () (const T &key1, const T &key2){
        return key1 == key2;
    }
};

std::ifstream getDataStream() {
    std::ifstream data;
    data.open("../test_data/cleaned_ingredients.csv");
    return data;
}


template <typename Value, typename Hash, typename KeyEqual>
void fillMapWithData(Hashmap<std::string, Value, Hash, KeyEqual> &map, std::ifstream &data) {
    if (std::is_integral_v<Value>) {
        std::string s;
        size_t i = 0;
        while (std::getline(data, s)) {
            map[s] = ++i; 
        }
    } else {
        std::string s;
        auto it = map.begin();
        cout << typeid(it).name() << endl;
        s = std::string("Value type '") + typeid((*it).second).name() + std::string("' is not iterable");
        throw std::invalid_argument(s);
    }
}

int main() {
    // unordered_map<std::string, size_t> map{
    //     pair("one", 1),
    //     pair("two", 2),
    //     pair("three", 3),
    //     pair("four", 4)
    // };
    // for (auto it = map.begin(); it != map.end(); ++it) {
    //     std::type_info(*it).name();
    // }
    // Hashmap<std::string, size_t>::iterator::value_type;

    // Node<int, int> node();

    // std::ifstream data;
    // data.open("cleaned_ingredients.csv");

    
    {
        Hashmap<int, int> map(2); // , hash<int>, equal<int>

        cout << endl;

        map[1] = 2;
        map[2] = 4;
        map[3] = 5;
        map[3] = 6;

        map.printBuckets();
        cout << endl;


        Hashmap<std::string, size_t> smap;
        Hashmap<std::string, float> fmap;

        auto data = getDataStream();
        fillMapWithData(smap, data);

        cout << smap.buckets_.sz << endl;
        cout << smap["P023,kalava,89.38600000000001,19.38,0.455,1.2,0.0,0.0,0.0,10.66,0.26,22.87,177.0,279.0,40.14,0.82,0.72,0.02,12.34,0.0,0.06,0.05,2.24,135.0,1194.0,0.0,0.0,0.33,0.0"];

        // for (auto it : smap) {
        //     cout << it.second <<  " ";
        // }

        
        // smap["sfsf"] = 2;
        // smap["sff"] = 4;
        // smap["sf"] = 4;

        // smap.printBuckets();
    }
    return 0;
}

