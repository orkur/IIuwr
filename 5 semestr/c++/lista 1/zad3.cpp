#include <iostream>
#include <set>
using stringset = std::set<std::string>;
int main()
{
    stringset strings {"a", "b", "c", "15", "10"};
    for (auto x : strings)
        std::cout << x << " ";

    return 0;
}