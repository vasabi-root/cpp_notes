#include <iostream>
#include <vector>
#include <set>

using std::ostream;
using std::istream;

using std::cout;
using std::cin;
using std::endl;

using std::vector;
using std::set;

template <typename T>
ostream& operator << (ostream &out, vector<T> vec) {
    out << "[ ";
    for (int64_t i = 0; i < (int64_t)vec.size(); ++i) {
        out << vec[i] << " ";
    }
    out << "]" << endl;

    return out;
}

ostream& operator << (ostream &out, int *vec) {
    out << "[ ";
    for (int64_t i = 0; i < 10; ++i) {
        out << vec[i] << " ";
    }
    out << "]" << endl;

    return out;
}

// template <typename T>
// ostream& operator << (ostream &out, set<T> s) {
//     set<T>::iterator it = s.begin();
//     out << "{ ";
//     for (; it != s.end(); ++it) {
//         out << *it << " ";
//     }
//     out << "}" << endl;

//     return out;
// }