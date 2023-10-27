#include <iostream>
#include <climits>
#include <limits>
#include <bitset>
int main()
{
    std::cout << "wartosc najblizsza 0 float -> " << std::numeric_limits<float>::min() << " double -> " << std::numeric_limits<double>::min() << "\n";
    std::cout << "wartosc max float -> " << std::numeric_limits<float>::max() << " double -> " << std::numeric_limits<double>::max() << "\n";
    std::cout << "najmniejsza roznica miedzy 1 float -> " << std::numeric_limits<float>::epsilon() << " double -> " << std::numeric_limits<double>::epsilon() << "\n";
    
}