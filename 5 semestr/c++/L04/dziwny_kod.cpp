#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <set>
#include <list>
template<typename T>
class Between {
    private:
        T p;
        T k;
    public:
        Between(T p, T k){
            this->p = p;
            this->k = k;
        }
        void operator() (T& x) {
            if(p < x && x < k) std::cout << x << " ";
        }
};
int main()
{
    double lower_bound = 0;
    double upper_bound = 100;
    std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
    std::default_random_engine re;
    std::vector<double> lista1;
    std::list<std::string> lista2;
    std::set<int> lista3;
    //std::vector<int> lista3;
    for(int i = 0 ; i < 20; i++){ 
        lista1.push_back(unif(re));
        std::string s = "";
        for(int j = 0; j < 10; j++) s += ('a' + ((int) std::floor(unif(re))%26));
        lista2.push_back(s);
        lista3.insert(std::floor(unif(re)));
        //lista3.push_back(std::floor(unif(re)));
    }
    std::cout << "Wygenerowane liczby:\n";
    for(auto i : lista1) std::cout << i << " ";
    std::cout << "\n";
    for(auto i : lista2) std::cout << i << " ";
    std::cout << "\n";
    for(auto i : lista3) std::cout << i << " ";
    std::cout << "\n\nelementy pomiedzy:\n";
    std::for_each(lista1.begin(), lista1.end(), Between(20.5, 70.5));
    std::cout << "\n";
    std::for_each(lista2.begin(), lista2.end(), Between(std::string("bbbaaaaaaa"), std::string("wyrewolwec")));
    std::cout << "\n";
    for(auto &i: lista3) std::cout << i << " ";
    //std::for_each(lista3.begin(), lista3.end(), [] (int &x) {std::cout << x << " ";});
    return 0;
}
