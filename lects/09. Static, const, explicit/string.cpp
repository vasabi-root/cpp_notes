#include <iostream>
#include <cstring>
#include <algorithm>

using std::cout;
using std::cin;
using std::endl;

class String {
private:
    size_t size_ = 0;
    char  *str_  = nullptr;

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
 
    ~String();

    String& operator = (String);
    const char& operator [] (const size_t&) const;  // только чтение 
    char& operator [] (const size_t&);              // чтение и запись
    
    size_t size() const { 
        return size_;
    }
    void swap(String &s) {
        std::swap(size_, s.size_);
        std::swap(str_, s.str_);
    }
};

// String::String(const size_t &size, const char &c='\0') {
//     this->size = size;
//     str = new char[size];
//     memset(str, c, size);
// }

String::String(const size_t &size, const char &c='\0') 
: size_(size)
, str_(new char[size]) {
    memset(str_, c, size_);
} 

// делегирование конструкторов с c++11
String::String(const String &s) : String(s.size_) {
    memcpy(str_, s.str_, size_*sizeof(*s.str_));
}

String::String(std::initializer_list<char> lst) {
    size_ = lst.size();
    std::copy(lst.begin(), lst.end(), str_);
}

String::~String() {
    delete [] str_;
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

const char& String::operator [] (const size_t &idx) const {
    return str_[idx];
}

char& String::operator [] (const size_t &idx) {
    return str_[idx];
}

std::ostream& operator << (std::ostream &out, const String &s) {
    for (size_t i=0; i < s.size(); ++i) {
        out << s[i];
    }
    return out;
}

std::istream& operator << (std::istream &in, String &s) {
    char c;
    while (c != '\n') {
        in >> c;

    }
    return in;
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