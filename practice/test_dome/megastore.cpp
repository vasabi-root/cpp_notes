#include <iostream>

class MegaStore
{
public:
    enum DiscountType 
    {
        Standard,
        Seasonal,
        Weight
    };
    
    static double getDiscountedPrice(double cartWeight, double totalPrice, DiscountType discountType)
    {
        double discounts[] = {0.06, 0.12, 0.06};
        switch (discountType) {
        case Standard:
            return (1-0.06)*totalPrice;
        case Seasonal:
            return (1-0.12)*totalPrice;
        case Weight:
            if (cartWeight > 10)
                return (1-0.18)*totalPrice;
            return (1-0.06)*totalPrice;
        }   
    }
};

#ifndef RunTests
int main()
{
    std::cout << MegaStore::getDiscountedPrice(12, 100, MegaStore::DiscountType::Weight);
    return 0;
}
#endif