#include <iostream>
#include <string>
#include <algorithm>
// #include "print.cpp"

using std::cout;
using std::cin;
using std::endl;
// using std::vector;
// using std::set;

int main() 
{
    int32_t nailNum = 0;        // количество гвоздей
    int32_t nailCoords[101] = {}; // вектор координат гвоздей

    // считываем входные данные (сразу сортируем координаты)
    cin >> nailNum;

    for (int32_t i = 1; i <= nailNum; ++i) {
        cin >> nailCoords[i];
    }
    std::sort(nailCoords, nailCoords + nailNum+1);
    
    // вектор состояний динамики (минимальная нить до i-того гвоздя)
    int32_t dp[101] = {}; 
    dp[1] = 0;
    dp[2] = nailCoords[2] - nailCoords[1];
    dp[3] = dp[2];

    // обратный обход
    for (int32_t i = 2; i < nailNum; ++i) {
        dp[i+1] += nailCoords[i+1] - nailCoords[i];
        dp[i+2] += std::min(dp[i], dp[i+1]);
    }
    cout << dp[nailNum] << endl;
	return 0;
}