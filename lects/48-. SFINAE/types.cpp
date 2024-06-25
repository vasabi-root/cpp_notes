#include <iostream>
// #include <type_traits>

struct X;            // declaration of X, no definition provided yet
extern X* xp;        // xp is a pointer to an incomplete type:
                     // the definition of X is not reachable
 
void foo()
{
    // xp++;            // ill-formed: X is incomplete
}
 
struct X { int i; }; // definition of X
X x;                 // OK: the definition of X is reachable
 
void bar()
{
    xp = &x;         // OK: type is “pointer to X”
    xp++;            // OK: X is complete
}

int main() {

}