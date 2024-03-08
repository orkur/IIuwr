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
        void operator() (T x) {
            if(p < x && x < k) std::cout << x << " ";
        }
};
class pk{
    private:
        int p;
        int k;
        int it = 0;
    public:
        pk(int p, int k){
            this->p = p;
            this->k = k;
        }
        template<typename T>
        void operator() (T x) {
            if(it >= p && (it - p) % k == 0) std::cout << x << " ";
            // std::cout << it << "\n";
            it++;
        }
};

template <template<class ...> class C, class V>
void mean(C<V> lista)
{
    int l = 0;
    V sum = 0;
    std::for_each(lista.begin(), lista.end(), [&sum, &l](V x) {
        sum += x;
        l++;
    });
    std::cout << sum/l << "\n";
}
template <template<class ...> class C, class V>
void minmax(C<V> lista)
{
    V mini;
    V maxi;
    bool set =0;
    std::for_each(lista.begin(), lista.end(), [&mini, &maxi, &set](V x) {
        if(!set)
        {
            set = 1;
            mini=maxi=x;
        }else{
        maxi = std::max(x, maxi);
        mini = std::min(x, mini);
        }
    });
    std::cout << mini << "   " << maxi << "\n";
}
template <template<class ...> class C, class V>
void suma(C<V> lista)
{
    V k;
    bool set =0;
    std::for_each(lista.begin(), lista.end(), [&k, &set](V x) {
        if(!set)
        {
            set = 1;
            k = x;
        }else
        k+=x;
    });
    std::cout << k << "\n";
}
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
    std::for_each(lista3.begin(), lista3.end(), Between(21, 40));
    std::cout << "\n\nwypisz co k od p:\n";
    std::for_each(lista1.begin(), lista1.end(), pk(1, 5));
    std::cout << "\n";
    std::for_each(lista2.begin(), lista2.end(), pk(0, 2));
    std::cout << "\n";
    std::for_each(lista3.begin(), lista3.end(), pk(5, 1));
    std::cout << "\n\nsrednia:\n";
    mean(lista1);
    mean(lista3);
    std::cout << "\n\nmin i max:\n";
    minmax(lista1);
    minmax(lista2);
    minmax(lista3);
    std::cout << "\n\nsuma/konkatenacja:\n";
    suma(lista1);
    suma(lista2);
    suma(lista3);
}