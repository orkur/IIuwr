#include <iostream>
#include <cmath>
using namespace std;
int main()
{
    for(int i = 0; i < 256; i+=4)
        cout << (int) exp(i/32) <<", ";
}
