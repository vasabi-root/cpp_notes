#include <iostream>
#include <iomanip>
#include <fstream>

using std::cin;
using std::cout;
using std::endl;

struct S1 {
    int vari;
    float varf;
};

extern S1 exs1; // не понятен порядок инициализации нелокальных статических объектов
// обращение к s1 в пользовательском коде (main_static) приведёт к UB

// решение: объявить статическую переменную внуктри функции 
// (сделать нелокальный объект локальным)
S1& s1() {
    static S1 var_s1;
    return var_s1;
}