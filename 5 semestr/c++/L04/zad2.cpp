#include <iostream>
#include <vector>
#include <algorithm>
template <class F, class G>
class po_kolei {
private:
    F f;
    G g;

public:

    po_kolei(F _f, G _g)
        : f(_f), g(_g) { }

    decltype(auto) operator() (auto x){
        f(x);
        g(x);
    }
};
int main()
{
    std::vector<int> wyj = {1, 2, 3, 4, 5};
    std::for_each(wyj.begin(), wyj.end(), 
            po_kolei(
                po_kolei(
                    [](int x) { std::cout << x + 1 << std::endl; },
                    [](int x) { std::cout << x + 2 << std::endl; }
                ),
                [](int x) { std::cout << x + 3 << std::endl; }
            )
        );
}