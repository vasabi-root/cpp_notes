#include <iostream>
#include <unordered_map>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <typeinfo>
#include <type_traits>
#include "../../bench.cpp"

// cd D:\Users\vasab\Documents\GitHub\cpp_notes\practice\hashmap\src
// g++ hashmap.cpp -std=c++20 -O0 -Wall -Wextra -g -o main; ./main

using std::cout;
using std::cin;
using std::endl;

using std::unordered_map;

template <
    typename Key,                           // ключ
    typename Value,                         // значение 
    typename Hash = std::hash<Key>,         // хэш-функция
    typename KeyEqual = std::equal_to<Key>, // функтор эквивалентности
    typename Allocator = std::allocator<std::pair<const Key, Value>>
>
class Hashmap {
private:
    using value_type = std::pair<const Key, Value>;
    using AllocTraits = std::allocator_traits<Allocator>;

    struct Node;
    struct List;

    Allocator allocator;
    Node **array_;
    List buckets_;

    size_t capacity_;
    double load_factor_;

    Node* insert_( Node *place, const Key &key, const Value &val=Value());
    
public:
    Hashmap(size_t capacity=256, double load_factor=1.0,
        Allocator allocator=Allocator())
    : 
    allocator(allocator),
    capacity_(0),               // именно 0, чтобы указать на инициализацию в rehash
    load_factor_(load_factor)
    { reserve(capacity); }

    ~Hashmap() {
        // buckets_.~List();
        // Bench bench;
        // bench.start();
        delete [] array_;
        // cout << "Destruct of Hashmap: " << bench.measure() << " s" << endl; 
    }

    Hash hashFunc;
    KeyEqual equalTo;

    struct iterator;

    iterator begin() { return iterator(buckets_.head->next); }
    iterator end() { return iterator(buckets_.head); }

    size_t getHash(const Key &key) { return hashFunc(key) % capacity_; }
    double getCurrentLoadFactor(size_t cap) { return static_cast<double>(buckets_.sz) / cap; }

    size_t size() { return buckets_.sz; }

    Value& operator [] (const Key &key);

    void rehash();

    void reserve(const size_t &count) { // count -- количество элементов, а не корзин
        size_t cap = std::ceil(count / load_factor_);
        if (getCurrentLoadFactor(cap) < load_factor_) {
            if (capacity_ > 0) {
                delete [] array_;
            } 
            array_ = new Node*[cap];           // есть смысл выделять под чары, ибо тогда не будет исключений
            for (size_t i = 0; i < cap; ++i) { // всё равно надо занулить эту область -- нам не нужен конструктор на данном этапе
                array_[i] = nullptr;
            }
            
            capacity_ = count;
            rehash();
        }
    }

    void printNodesWithHash(size_t hash) {
        Node *node = array_[hash];
        while (node->hash == hash) { 
            cout << *node << endl;
            node = node->next;
        }
    }

    void printNodesWithKey(Key key) {
        Node *node = buckets_.head->next;
        while (node != buckets_.head) { 
            if (equalTo(node->kv.first, key)) {
                cout << *node << endl;
            }
            node = node->next;
        }
    }

    void printBuckets() {
        Node *node = buckets_.head->next; 

        while (node != buckets_.head) {
            cout << node->hash << ": [" << node->kv.first << " " << node->kv.second << "]" << endl; 
            node = node->next;
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


template <typename Key, typename Value, typename Hash, typename KeyEqual, typename Allocator>
struct Hashmap<Key, Value, Hash, KeyEqual, Allocator>::Node {
    Node *prev;
    Node *next;
    size_t hash;
    value_type kv;

    Node (value_type kv=value_type(Key(), Value()), const size_t &hash=0)
    : 
    prev(this), next(this),
    hash(hash), kv(kv)
    {}

    friend std::ostream& operator << (std::ostream &out, const Node &node) {
        Hash getHash;
        // out << "{ " << node.kv.first;
        for (auto n : {*node.prev, node, *node.next}) {
            out << " key ";
            out << " | " << n.kv.second;
            out << "\t | " << n.hash;
            out << "\t | " << getHash(n.kv.first) << " }" << endl;
        }
        out << endl;

        return out;
    }
};

template <typename Key, typename Value, typename Hash, typename KeyEqual, typename Allocator>
struct Hashmap<Key, Value, Hash, KeyEqual, Allocator>::iterator {        
private:
    Node *node;
public:
    // iterator tags
    using value_type        = value_type;
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using pointer           = value_type*;
    using reference         = value_type&;

    iterator(Node *n) : node(n) {};

    reference operator * () const { node->kv; }
    // pointer* operator -> () const { return static_cast<Node*>(node)->kv; }
    iterator& operator ++ () { node = node->next; return *this; }
    iterator& operator ++ (int) { iterator tmp = *this; ++(*this); return *tmp; }

    friend bool operator == (const iterator &it1, const iterator &it2) { return it1.node == it2.node; }
    friend bool operator != (const iterator &it1, const iterator &it2) { return it1.node != it2.node; }
};

template <typename Key, typename Value, typename Hash, typename KeyEqual, typename Allocator>
struct Hashmap<Key, Value, Hash, KeyEqual, Allocator>::List {
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
        cout << head->hash << " -> ";
        while (node != head) {
            cout << node->kv.first << " -> ";
            node = node->next;
        }
        cout << head->hash << endl;
    }
};

template <typename Key, typename Value, typename Hash, typename KeyEqual, typename Allocator>
typename Hashmap<Key, Value, Hash, KeyEqual, Allocator>::
Node* Hashmap<Key, Value, Hash, KeyEqual, Allocator>::
insert_(Node *place, const Key &key, const Value &val) {
    Node *node = new Node({key, val}, getHash(key));
    buckets_.insert(place, node);
    if (getCurrentLoadFactor(capacity_) > load_factor_) {
        cout << "REHASH: " << capacity_ << " -> ";
        reserve(capacity_*2);
        cout << capacity_ << endl;
    }
    return node;
}

template <typename Key, typename Value, typename Hash, typename KeyEqual, typename Allocator>
Value& Hashmap<Key, Value, Hash, KeyEqual, Allocator>::operator [] (const Key &key) {
    size_t hash = getHash(key);
    Node *place = array_[hash];

    if (place != nullptr) {
        while (
            place->next != buckets_.head &&     // пока не дошли до конца
            place->hash == hash &&              // хэш соответствует посчитанному
            !equalTo(place->kv.first, key)     // и ключ не совпадает с нужным
        ) {
            place = place->next;
        }

        if (
            // если мы дошли до конца и ключ всё ещё не совпадает
            (place->next == buckets_.head && !equalTo(place->kv.first, key)) 
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

template <typename Key, typename Value, typename Hash, typename KeyEqual, typename Allocator>
void Hashmap<Key, Value, Hash, KeyEqual, Allocator>::rehash() {
    Node *curr = buckets_.head->next;

    while (curr != buckets_.head) {
        size_t hash = getHash(curr->kv.first);
        Node *next = curr->next;

        curr->hash = hash;

        if (array_[hash] != nullptr && array_[hash] != curr) {
            curr = buckets_.erase(curr);
            buckets_.insert(array_[hash], curr);
        } 
        array_[hash] = curr; // IMPORTANT !!!
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
    // data.open("../test_data/cleaned_ingredients.csv");
    data.open("../test_data/ingredient_6L.csv");
    // data.open("../test_data/test.csv");
    return data;
}


template <
    template <class, class, class, class, class> typename Map, 
    typename Value, typename Hash, typename KeyEqual, typename Allocator
>
void fillMapWithData(Map<std::string, Value, Hash, KeyEqual, Allocator> &map, std::ifstream &data) {
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
    // {
    //     unordered_map<std::string, size_t> map{
    //         {"one", 1},
    //         {"two", 2},
    //         {"three", 3},
    //         {"four", 4}
    //     };

    //     // auto it = map.begin();
    //     unordered_map<std::string, size_t>::iterator it = map.begin();
    //     cout << typeid(it).name() << endl;
    //     // Hashmap<std::string, size_t>::iterator::value_type;
    // }

    {
        Bench bench;
        // Hashmap<int, int> map(2); // , hash<int>, equal<int>

        // cout << endl;

        // map[1] = 2;
        // map[2] = 4;
        // map[3] = 5;
        // map[3] = 6;

        // map.printBuckets();
        // cout << endl;
        // for (auto it : map) {
        //     cout << it.second <<  " ";
        // }

        Hashmap<std::string, size_t> smap(256);
        unordered_map<std::string, size_t> sumap;

        auto data = getDataStream();
        bench.start();
        fillMapWithData(smap, data);
        cout << endl;
        cout << "Hashmap : " << bench.measure() << " s" << endl;

        data.close();
        data = getDataStream();
        bench.start();
        fillMapWithData(sumap, data);
        cout << "stl::map: " << bench.measure() << " s" << endl;

        cout << "hashmap : " << smap["6037c3588fbc30d3db70d351,Overripe bananas big"] << endl;
        cout << "stl::map: " << sumap["6037c3588fbc30d3db70d351,Overripe bananas big"] << endl;
        cout << "hashmap : " << smap["S010,tiger prawns,67.876,14.24,0.134,0.66,0.0,0.0,0.0,57.9,0.84,22.94,155.0,149.0,80.77,1.02,0.39,0.08,14.69,0.0,0.01,0.04,1.28,216.0,1875.0,0.0,0.0,1.65,0.0"] << endl;
        cout << "stl::map: " << sumap["S010,tiger prawns,67.876,14.24,0.134,0.66,0.0,0.0,0.0,57.9,0.84,22.94,155.0,149.0,80.77,1.02,0.39,0.08,14.69,0.0,0.01,0.04,1.28,216.0,1875.0,0.0,0.0,1.65,0.0"] << endl;

        cout << endl;
        cout << smap.size() << endl;
        cout << sumap.size() << endl;

        cout << endl;

        
        // smap["sfsf"] = 2;
        // smap["sff"] = 4;
        // smap["sf"] = 4;

        // smap.printBuckets();
    } 
    return 0;
}

