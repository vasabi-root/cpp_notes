#include <iostream>
#include <vector>

using std::cout;
using std::cin;
using std::endl;

// using std::vector;

template <
    typename Key,                   // ключ
    typename Value,                 // значение 
    typename Compare=std::less<Key> // комапатор (функтор отношения порядка!)
>
class map {
    using value_type = std::pair<sonst Key, Value>;

    struct BaseNode {
        Node *left;
        Node *right;
        Node *parent;

        bool red;
    };  

    struct Node: BaseNode {
        value_type kv;
    };

public:
    struct iterator
    {
        
        BaseNode *node;
        value_type& operator * () const {
            return node.kv;
        }
    };
    
};


int main() {

    return 0;
}

