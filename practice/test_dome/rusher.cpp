#include <vector>
#include <iostream>

using std::cout;
using std::endl;

class GamePlatform 
{
public:
    static double calculateFinalSpeed(double initialSpeed, const std::vector<int>& inclinations) 
    {
        for (auto inc : inclinations) {
            initialSpeed += -inc;
            if (initialSpeed <= 0)
                return 0;
        }
        return initialSpeed;
    }
};
#ifndef RunTests
int main()
{
    std::cout << GamePlatform::calculateFinalSpeed(60.0, { 0, 30, 0, -45, 0 });
    return 0;
}
#endif