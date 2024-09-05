#include <iostream>
#include <cstring>

void foo(unsigned char) {}
void foo(unsigned int) {}
void a(const char *str)
{ 
//   foo(strlen(str)); // CE
}

struct S {
    
    void func(int) { std::cout << "sdfsdf\n"; }
};

typedef void (S::*pfunc)(int);

int main()
{
    {
        // a("sdfsdf"); // CE
        pfunc f = &S::func;
        S a;
        (a.*f)(9);
        
        pfunc *fs = new pfunc [2];
        fs[0] = &S::func;
        fs[1] = &S::func;
        
        std::cout << sizeof(f) << std::endl;     // 16
        std::cout << sizeof(fs) << std::endl;    // 8
        std::cout << sizeof(fs[0]) << std::endl; // 16
        
        (a.*fs[0])(0);
        (a.*fs[1])(0);
        
        delete [] fs;
    }
    {   // swap through XOR
        int a = 9, b = 10;
        a ^= b; 
        b ^= a; // b = b^(a^b) = a;
        a ^= b; // a = (a^b)^a = b;

        int *pa = &a;
        int *& v = pa;
        
        std::cout << a << " " << b << std::endl;
    }
    

    return 0;
}