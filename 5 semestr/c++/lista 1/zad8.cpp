#include <iostream>
#include <deque>
#include <tuple>
#include <algorithm>
class osoba{
    public:
        std::string nazwisko, imie, data;
    osoba(std::string n, std::string i, std::string d){
        nazwisko = n;
        imie = i;
        data = d;

    }
    bool operator<(osoba const & b){
        std::tuple a1 = std::tie(nazwisko, imie, data);
        std::tuple b1 = std::tie(b.nazwisko, b.imie, b.data);
        return a1 < b1;
    }
};
// bool comp(osoba a, osoba b)
// {
//     std::tuple a1 = std::tie(a.nazwisko, a.imie, a.data);
//     std::tuple b1 = std::tie(b.nazwisko, b.imie, b.data);
//     return a1 < b1;
// }
int main(){
    std::deque< osoba > osoby;
    osoby.push_back(osoba("Galczewski", "Jakub", "2002"));
    osoby.push_back(osoba("Galczewski", "Amadeusz", "2009"));
    osoby.push_back(osoba("Aron", "Janina", "1998"));
    osoby.push_back(osoba("Lizon", "Amelia", "2001"));
    sort(osoby.begin(), osoby.end());
    for(auto i : osoby){
        // std:: string a, b, c;
        // std:: tie(a, b, c) = i;
        auto[a, b, c] = i;
        std:: cout << a << " " << b << " " << c << "\n";
    }

}