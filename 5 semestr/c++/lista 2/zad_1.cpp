#include <iostream>
#include <cstdint>
#include <memory>
#include <stdlib.h>
#include <time.h>
class drobna{
	public:
		uint64_t licznik;
		drobna(){
			licznik = 1;
		}
		virtual ~drobna(){
			std::cout << "rozmiar licznika wynosi " << licznik << "\n";
		}
};
void dodaj(int m, int n, std::unique_ptr<drobna[]> &tab, int i = 0)
{
	if(m == i) return;
	for(int j = 0; j < n/10; j++){
		int n1 = rand()%n;
		tab[n1].licznik += i;
	}
	dodaj(m, n, tab, i+1);
}
int main(){
	srand (time(NULL));
	int n, m;
	std::cout << "Podaj n i m: ";
	std::cin >> n >> m;
	std::unique_ptr<drobna[]> tab(new drobna[n]);
	dodaj(m, n, tab);
	for(int i = 0; i < n; i++)
		std::cout << tab[i].licznik << " ";
	std::cout<<"\n";
	
}
