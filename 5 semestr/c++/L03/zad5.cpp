#include <iostream>
#include <random>
#include <chrono>
class macierz{
    private:
        int n;
        double **tab;
        macierz(int x, int st)
        {
            n = x;
            tab = new double*[x];
            for(int i = 0; i < x; i++) tab[i] = new double[x];
            for(int i = 0; i < x; i++){
                for(int j = 0; j < x; j++){
                    tab[i][j] = st;
                }
            }
        }
    public:
        macierz(int x)
        {
            n = x;
            tab = new double*[x];
            for(int i = 0; i < x; i++) tab[i] = new double[x];
            double lower = 0.5;
            double upper = 2.0;
            std::uniform_real_distribution<double> unif(lower, upper);
            std::default_random_engine r;
            for(int i = 0; i < x; i++){
                for(int j = 0; j < x; j++){
                    tab[i][j] = unif(r);
                }
            }
        }
        friend macierz operator*(const macierz &a, const macierz &b)
        {
            macierz c(a.n, 0);
            for(int i = 0; i < c.n; i++)
                for(int j = 0; j < c.n; j++)
                    for(int k = 0; k < c.n; k++)
                        c.tab[i][j] += a.tab[i][k] * b.tab[k][j];
            return c;
        }
        void wypisz(){
            for(int i = 0; i < n; i++){
                for(int j = 0; j < n; j++){
                    std::cout << tab[i][j] << " ";
                }
                std::cout << "\n";
            }
        }
};
void testMacierzy(int n, int powt)
{
    macierz a(n);
    macierz c = a;
    std::chrono::high_resolution_clock tmpClock;
    auto start = tmpClock.now();
    for(int i = 0; i < powt; i++)
    {
        a = a * c;
    }
    auto end = tmpClock.now();
    auto difference = end - start;
    std::cout << "czas dla macierzy kwadratowej o rozmiarze: " << n << " wynosi: " << (difference/std::chrono::milliseconds(1))/powt << " milisekund\n";
}
int main(){
    testMacierzy(10, 100000);
    testMacierzy(100, 1);
    testMacierzy(1000, 1);
}