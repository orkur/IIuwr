#include <iostream>
#include <cstdint>

decltype(auto) Lucas(uint64_t n){ //działa ale wolno
    if(n == 0)
        return 2;
    if(n == 1)
        return 1;
    return Lucas(n-1) + Lucas(n-2);
}

decltype(auto) Lucasopt(uint64_t n, uint64_t l0, uint64_t l1){
    if(n == 0)
        return l0;
    if(n == 1)
        return l1;
    return Lucasopt(n - 1, l1, l0+l1);
}

decltype(auto) Lucas2(uint64_t n){
    return Lucasopt(n, 2, 1);
}

int main()
{
    for(int i = 0; i < 30; i++) std::cout << Lucas(i) << " ";

    std::cout << "\n";

    for(int i = 0; i < 30; i++) std::cout << Lucas2(i) << " ";   
}