#include <iostream>
#include <climits>
#include <limits>
#include <bitset>
int main()
{
    long long int x;
    x = LLONG_MAX;
    std::cout << "rozmiar long longa -> " << sizeof(long long int) * 8 << " " << std::numeric_limits<long long int>::digits << " " << std::numeric_limits<long long int>::digits10 <<"\n";
    std::cout << "najwieksza wartosc -> " << std::numeric_limits<long long int>::max() << " " << LLONG_MAX << "\n";
    std::cout << std::bitset<sizeof(long long int) * 8>(x) << "\n";
    std::cout << "najmniejsza wartosc -> " << std::numeric_limits<long long int>::min() << " " << LLONG_MIN << "\n";
    std::cout << std::bitset<sizeof(long long int) * 8>(x+1) << "\n";

}