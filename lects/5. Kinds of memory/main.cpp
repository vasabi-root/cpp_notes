#include <iostream>
using std::cout;
using std::cin;

void f(float n) {}
void g(int, char) {}

int main() {
    int c = 3;
    cout << &c << '\n'; 
    {
        double a = 15.3;
        cout << &a << '\n';
    }
    double *d = (double*)(&c-3);
    cout << d << '\n';
    cout << *d << '\n';
    {
        int b;
        cout << &b << '\n';
    }
    cout << sizeof(double) << '\n';

    {
        int *p = new int(10); 
    } // memory leak

    int arr[] = {1, 2, 3, 4, 5};

    // f(arr); // [arr] = [int*]
    // f(&arr); // [&arr] = int(*)[5]

    // a[b] = *(a+b)
    cout << 3[arr] << " " << arr[3] << '\n';

    int *k = new int(100); // один int со значением 100
    int *m = new int[100]; // 100 int 

    delete k;
    delete[] m; 

    int n = 3;
    // int *p = new int[n*n]; 
    // int p[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int *p = new int[n*n];
    int **pp = (int**) new int[n];
    
    for (int i = 0; i < n; ++i) {
        pp[i] = (int*)(p + n*i);
        for (int j = 0; j < n; ++j) {
            p[i*n + j] = i*n +j +1;
            cout << pp[i][j] << ' ';
        }
        cout << '\n';
    }

    delete []p;
    delete []pp;

    // указатель на функцию
    void (*pg)(int, char) = &g;  // одно
    void (*pg)(int, char) = g;   // и то же
      

    return 0;
}