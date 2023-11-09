#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <set>
#include <list>
class PrintVal {
    private:
        int pos;
        int p;
        int k;
    public:
        PrintVal(int p = 0, int k){
            this.pos = p;
            this.p = p;
            this.k = k;
        }
        void operator() (int& elem) const {
            if((pos - p) % k == 0) std::cout <<     
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
    for(int i = 0 ; i < 20; i++){ 
        lista1.push_back(unif(re));
        std::string s = "";
        for(int j = 0; j < 10; j++) s += ('a' + ((int) std::floor(unif(re))%26));
        lista2.push_back(s);
        lista3.insert(std::floor(unif(re)));
    }
    for(auto i : lista1) std::cout << i << " ";
    std::cout << "\n";
    for(auto i : lista2) std::cout << i << " ";
    std::cout << "\n";
    for(auto i : lista3) std::cout << i << " ";
    std::cout << "\n";
    
}