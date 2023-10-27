#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <set>
enum plec_lamy
{
    piekny,
    piekna
};
class llama{
	std::string imie;
    std::string kolor;
    plec_lamy plec;
    std::shared_ptr<llama> mama;
    std::shared_ptr<llama> tata;
    std::vector<std::weak_ptr<llama>> dzieci;
    public:
        std::string getImie()
        {
            return imie;
        }
        llama(std::string imie){this->imie = imie;}
        llama(std::string imie, std::string kolor, plec_lamy plec, std::shared_ptr<llama> mama, std::shared_ptr<llama> tata){
            this->imie = imie;
            this->kolor = kolor;
            this->plec = plec;

            this->mama = mama;
            this->tata = tata;
        }
        ~llama()
        {
            std::cout << "lama " << imie << " o umaszczeniu " << kolor << " odchodzi z naszej pamieci. Do zobaczenia!\n";
        }
        void ustawdziecko(std::shared_ptr<llama> dziecko)
        {
            dzieci.push_back(dziecko);
        }
    friend bool operator< (const llama& first, const llama& second)
    {
        return first.imie < second.imie;
    }
};
std::shared_ptr<llama> kupllama(std::string imie, std::string kolor, plec_lamy plec){
            std::cout << "gratulujemy pieknego zakupu! Imie to " << imie<<  "!\n";
            return std::make_shared<llama> (imie, kolor, plec, nullptr, nullptr);
}
std::shared_ptr<llama> nowallama(std::string imie, std::string kolor, plec_lamy plec, std::shared_ptr<llama> mama, std::shared_ptr<llama> tata){
    std::shared_ptr<llama> dziecko = std::make_shared<llama>(imie, kolor, plec, mama, tata);
    mama->ustawdziecko(dziecko);
    tata->ustawdziecko(dziecko);
    std::cout << "niech rosnie duze i zdrowe! Imie jego to " << imie <<"!\n";
    return dziecko;
}
void usun(std::string name, std::set<std::shared_ptr<llama>>& hodowla){
    for(auto i : hodowla)
        if(i->getImie() == name)
        {
            hodowla.erase(i);
            std::cout << name << " zostal usuniety\n";
            return;
        }
    
}
int main(){
    std::set<std::shared_ptr<llama>> hodowla;
    auto Maria = kupllama("maria", "bialy", piekna);
    hodowla.insert(Maria);
    hodowla.insert(kupllama("janek", "czarny", piekny));
    auto zbyszek = kupllama("zbyszek", "czarny", piekny);
    hodowla.insert(zbyszek);
    auto kuba = nowallama("kuba", "zielen", piekny, Maria, zbyszek);
    hodowla.insert(kuba);
    hodowla.erase(zbyszek); //ojciec kuby, nie znika
    zbyszek = nullptr;
    std::cout << "zbyszek jeszcze nie zniknal z naszej pamieci!\n";
    usun("janek", hodowla); //janek nie ma nikogo, zostanie zapomniany
    std::cout << "zwierzatka\n";
    for(auto it : hodowla)
        std::cout << it->getImie() << " ";
    std::cout << "\n";
    std::cout << "papa!\n";
}