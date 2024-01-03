#include <iostream>
#include <string>
#include <algorithm>

#include <fstream>
#include "print.cpp"


using std::cout;
using std::cin;
using std::endl;
// using std::vector;
// using std::set;

int main() 
{
    std::ifstream in(get_input());
    std::streambuf *cinbuf = std::cin.rdbuf(); //save old buf
    std::cin.rdbuf(in.rdbuf()); // redirect

    int16_t n, m;                    // размер матрицы
    int16_t fines[21][21] = {}; // матрица штрафов

    // считываем входные данные
    cin >> n >> m;
    for (int16_t i = 1; i <= n; ++i) 
        for (int16_t j = 1; j <= m; ++j) 
            cin >> fines[i][j];
    
    // таблица состояний динамики
    int16_t dp[21][21] = {}; 
    // инициализация
    dp[1][1] = fines[1][1];

    for (int16_t i = 2; i <= n; ++i) 
        dp[i][1] = dp[i-1][1] + fines[i][1];
    for (int16_t j = 2; j <= m; ++j) 
        dp[1][j] = dp[1][j-1] + fines[1][j];
    
    // обратный обход
    for (int16_t k = 2; k <= std::min(n, m); ++k) {
        for (int16_t i = 2; i <= n; ++i)
            dp[i][k] = std::min(dp[i-1][k], dp[i][k-1]) + fines[i][k];
        for (int16_t j = 2; j <= m; ++j)
            dp[k][j] = std::min(dp[k-1][j], dp[k][j-1]) + fines[k][j];
    }
    cout << dp[n][m] << endl;

    std::cin.rdbuf(cinbuf);
	return 0;
} 


/*
#include <iostream>
#include <string>
#include <algorithm>


using std::cout;
using std::cin;
using std::endl;

int main() 
{
    int16_t n, m;                    // размер матрицы
    int16_t fines[21][21] = {}; // матрица штрафов

    // считываем входные данные
    cin >> n >> m;
    for (int16_t i = 1; i <= n; ++i) 
        for (int16_t j = 1; j <= m; ++j) 
            cin >> fines[i][j];
    
    // таблица состояний динамики
    int16_t dp[21][21] = {}; 
    // инициализация
    dp[1][1] = fines[1][1];
    for (int16_t i = 2; i <= n; ++i) 
        dp[i][1] = dp[i-1][1] + fines[i][1];
    for (int16_t j = 2; j <= n; ++j) 
        dp[1][j] = dp[1][j-1] + fines[1][j];
    
    // прямой обход
    for (int16_t i = 2; i <= n; ++i) 
        for (int16_t j = 2; j <= m; ++j)
            dp[i][j] = std::min(dp[i-1][j], dp[i][j-1]) + fines[i][j];

    cout << dp[n][m] << endl;
	return 0;
} */
