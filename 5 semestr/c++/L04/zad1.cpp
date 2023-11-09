#include <iostream>
#include <vector>
#include <algorithm>
template< int (*f)(int, int), int (*gx)(int), int (*hx)(int)>
void compose_f_gx_hx(int& x)
{
    x = f(gx(x), hx(x));
}
int square(int x)
{
    return x*x;
}
int power_of_two(int x){
    int y = 1;
    while(x--) y *= 2;
    return y;
}
int suma(int a, int b)
{
    return a + b;
}
int main()
{
    std::vector<int> licz{1,2,3,4,5,6,7,8};
    for(auto i : licz) std::cout << i << " ";
    std::cout << "\n";
    std::for_each(licz.begin(), licz.end(), compose_f_gx_hx<suma, square, power_of_two>);
    for(auto i : licz) std::cout << i << " ";
}