#include <iostream>
#include <vector>

using std::cout;
using std::cin;
using std::endl;

using std::vector;

template <typename Iter> // шаблонный параметр называются по виду итератора
void process(Iter begin, Iter end) {
    --end; // [begin, end)
    typename std::iterator_traits<Iter>::value_type tmp = *begin;
    *begin = *end;
    *end = tmp;
    if (std::is_base_of<std::iterator_traits<Iter>::iterator_category, 
                        std::random_access_iterator_tag>){}
}

int main() {
    return 0;
}