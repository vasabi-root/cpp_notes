#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <windows.h>

using std::ostream;
using std::istream;

using std::cout;
using std::cin;
using std::endl;

using std::string;

using std::vector;
using std::set;

string get_current_dir() {
    char buff[MAX_PATH];
    GetModuleFileName( NULL, buff, MAX_PATH );
    string::size_type position = string( buff ).find_last_of( "\\/" );
    return string( buff ).substr( 0, position);
}

string get_input(const string &in="input.txt") {
    string dir = get_current_dir();
    return dir + '\\' + in;
}

template <typename T>
ostream& operator << (ostream &out, vector<T> vec) {
    out << "[ ";
    for (int64_t i = 0; i < (int64_t)vec.size(); ++i) {
        out << vec[i] << " ";
    }
    out << "]" << endl;

    return out;
}

ostream& operator << (ostream &out, int16_t *vec) {
    out << "[ ";
    for (int64_t i = 1; i <= 5; ++i) {
        out << vec[i] << " ";
    }
    out << "]" << endl;

    return out;
}

ostream& operator << (ostream &out, int16_t (*vec)[21]) {
    out << "[";
    for (int64_t i = 1; i <= 5; ++i) {
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