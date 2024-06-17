#include <iostream>
#include <cstring>
#include <algorithm>

using std::cout;
using std::cin;
using std::endl;

class String {
private:
    size_t size = 0;
    char  *str  = nullptr;

public:
    // String() {}

    // since c++11
    String() = default; // по умолчанию
    // String(int, char) = delete; // запрет такого конструктора
    String(const size_t &size, const char &c);
    String(const String &); // конструктор копии
    // since c++11
    String(std::initializer_list<char> lst);

    // String& operator= (const String &s);
    String& operator= (String);
 
    ~String();

    void swap(String &s) {
        std::swap(size, s.size);
        std::swap(str, s.str);
    }
};

// String::String(const size_t &size, const char &c='\0') {
//     this->size = size;
//     str = new char[size];
//     memset(str, c, size);
// }

String::String(const size_t &size, const char &c='\0') 
: size(size)
, str(new char[size]) {
    memset(str, c, size);
} 

// делегирование конструкторов с c++11
String::String(const String &s) : String(s.size) {
    memcpy(str, s.str, size*sizeof(*s.str));
}

String::String(std::initializer_list<char> lst) {
    size = lst.size();
    std::copy(lst.begin(), lst.end(), str);
}

String& String::operator= (String s) {
    // delete [] str;
    // size = s.size;
    // str = new char[size];
    // // memcpy(s.str, str, size * sizeof(*str));
    // std::copy(s.str, s.str+size, str);
    swap(s);
    return *this;
}

String::~String() {
    delete [] str;
}

int main() {
    String s1;
    // String s2(10, 'c');
    String s3 = 10;
    String s4 = String(10);

    {
        int *a = new int[10];
        cout << sizeof(*a) << endl;
    }
    {
        char *a = new char(10);
        cout << sizeof(*a) << endl;
    }
    return 0;
}