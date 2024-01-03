#include <iostream>
#include <unordered_map>
#include <iomanip>
#include <fstream>

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
    List buckets_;
    size_t *loads;

    size_t capacity_;
    double avg_load_;
    double load_factor_;

public:
    Hashmap(size_t capacity=256, double load_factor=3.5)
    : 
    capacity_(0),
    avg_load_(0.0),
    load_factor_(load_factor) 
    { reserve(capacity); }

    ~Hashmap() {
        buckets_.~List();
        delete [] array_;
    }

    Hash hashFunc;
    KeyEqual equal_to;

    struct iterator;

    iterator begin() { return iterator(buckets_.head.next); }
    iterator end() { return iterator(buckets_.head); }

    size_t get_hash(const Key &key) { return hashFunc(key) % capacity_; }

    Value& operator [] (const Key &key);

    void rehash();

    void reserve(const size_t &count) {
        if (capacity_ > 0) {
            delete [] array_;
        }
        array_ = new Node*[count];
        for (size_t i = 0; i < count; ++i) {
            array_[i] = nullptr;
        }

        capacity_ = count;

        rehash();
    }

    void printBuckets() {
        Node *node = buckets_.head.next; 
        size_t i = 0;

        while (node != &(buckets_.head) && node != nullptr) {
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
        cout << endl;
    }
    
};


template <typename Key, typename Value, typename Hash, typename KeyEqual>
struct Hashmap<Key, Value, Hash, KeyEqual>::Node {
    Node *prev = nullptr;
    Node *next = nullptr;
    value_type kv;
    size_t hash;

    Node (value_type kv=value_type(Key(), Value()), const size_t &hash=0): kv(kv), hash(hash) {}
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
    reference operator * () const { return static_cast<Node*>(node)->kv; }
    // pointer* operator -> () const { return static_cast<Node*>(node)->kv; }
    iterator& operator ++ () { node = node->next; return *this; }
    iterator& operator ++ (int) { iterator tmp = *this; ++(*this); return *tmp; }

    friend bool operator == (const iterator &it1, const iterator &it2) { return it1.node == it2.node; }
    friend bool operator != (const iterator &it1, const iterator &it2) { return it1.node != it2.node; }
};

template <typename Key, typename Value, typename Hash, typename KeyEqual>
struct Hashmap<Key, Value, Hash, KeyEqual>::List {
    Node head;
    size_t sz;

    List() { sz = 0; }
    ~List() { retract_all(); }

    void push_front(Node *node) { insert(head.next, node); }
    void push_back(Node *node) { insert(&head, node); }

    Node* erase(Node *node) { // требуется, чтобы node был в списке
        Node *prev = node->prev;
        Node *next = node->next;

        if (prev == next) {
            head.next = nullptr;
            head.prev = nullptr;
        } else {
            prev->next = next;
            next->prev = prev;
        }
        printBuckets();
        --sz;

        // delete node;
        return node;
    }

    void insert(Node *place, Node *node) {
        Node *prev = place->prev;

        if (prev == nullptr) { // empty list
            cout << "push_back" << endl;
            head.next = node;
            head.prev = node;

            node->next = &head;
            node->prev = &head;
        } else {
            cout << "push_back" << endl;

            prev->next = node;

            node->prev = prev;
            node->next = place;

            place->prev = node;
        }

        printBuckets();
        ++sz;
    }

    void retract_all() {
        while (head.next != nullptr) {
            delete erase(head.next);
        }
    }

    void printBuckets() {
        Node *node = head.next; 
        size_t i = 0;
        cout << head.hash << " -> ";
        while (node != &(head) && node != nullptr) {
            // cout << i << ": [" << node->kv.first << " " << node->kv.second << "][" << node->hash << "]" << endl; 
            cout << node->kv.first << " -> ";
            node = node->next;
            ++i;
        }
        cout << head.hash << endl;
    }
};

using pair = std::pair<std::string, size_t>;

template <typename T>
struct hash {
    size_t operator () (const T &key){
        return *reinterpret_cast<const size_t*>(&key);
    }
};

template <typename Key, typename Value, typename Hash, typename KeyEqual>
Value& Hashmap<Key, Value, Hash, KeyEqual>::operator [] (const Key &key) {
    size_t hash = get_hash(key);
    Node *place = array_[hash];

    if (place != nullptr) {
        while (
            place->next != &buckets_.head &&    // пока не дошли до конца
            place->hash == hash &&              // хэш соответствует посчитанному
            !equal_to(place->kv.first, key)     // и ключ не совпадает с нужным
        ) {
            place = place->next;
        }

        if (
            // есди мы дошли до конца и ключ всё ещё не совпадает
            (place->next == &buckets_.head && !equal_to(place->kv.first, key)) 
            // или мы уже дошли до конца корзины, то вершины нет -- надо создать новую
            || place->hash != hash) {
            Node *node = new Node({key, Value()}, hash);
            buckets_.insert(place, node);
            place = node;
        } 
    } else {
        place = new Node({key, Value()}, hash);
        buckets_.push_back(place);
        array_[hash] = place;
        printBuckets();
    }
    cout << endl;
    return place->kv.second;
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
void Hashmap<Key, Value, Hash, KeyEqual>::rehash() {
    Node *curr = buckets_.head.next;
    List buckets_new;

    if (curr != nullptr) {
        while (curr != &buckets_.head) {
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
};

template <typename T>
struct equal {
    bool operator () (const T &key1, const T &key2){
        return key1 == key2;
    }
};

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
        Hashmap<int, int> map; // , hash<int>, equal<int>

        cout << endl;

        map[1] = 2;
        map[2] = 4;
        map[3] = 5;
        map[3] = 6;

        map.printBuckets();

        map.reserve(2);

        map.printBuckets();

        std::string s;

        Hashmap<std::string, size_t> smap;

        // size_t i = 0;
        // while (std::getline(data, s)) {
        //     smap[s] = ++i; 
        // }
        
        
        smap["sfsf"] = 2;
        smap["sff"] = 4;
        smap["sf"] = 4;

        smap.reserve(2);

        smap.printBuckets();

        // cout << map[5] << endl;
        // cout << map[2] << endl;
    }
    // Hashmap<int, int>::List lst;
    // using Node = Hashmap<int, int>::Node;
    // Node *node1 = new Node({1, 0}, 1);
    // Node *node2 = new Node({2, 3}, 4);
    // lst.push_back(node1);
    // lst.push_back(node2);
    // cout << endl;
    // // lst.erase(node2);
    // // lst.erase(node1);
    // lst.printBuckets();
    // // lst.push_back(&Hashmap<int, int>::Node({1, 0}, 0));


    return 0;
}

