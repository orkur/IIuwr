#include <iostream>

typedef struct  date
{
    int day, month, year;
}date;
std::string miesiac(int i){
    switch (i)
    {
    case 1:
        return "stycznia";
        break;
    case 2:
        return "lutego";
        break;
    case 3:
        return "marca";
        break;
    case 4:
        return "kwietnia";
        break;
    case 5:
        return "maja";
        break;
    case 6:
        return "czerwca";
        break;
    case 7:
        return "lipca";
        break;
    case 8:
        return "sierpnia";
        break;
    case 9:
        return "września";
        break;
    case 10:
        return "października";
        break;
    case 11:
        return "listopada";
        break;
    case 12:
        return "grudnia";
        break;
    }
}

date parse_date(std::string input){
    int data[3] = {};
    char kropka = '.';
    int j = 0;
    std::string s = "";
    int indeks = 0;
    while(j <= input.size())
    {
        if(input[j] == kropka || j == input.size()){
            data[indeks] = stoi(s);
            s = "";
            indeks++;
        }else{
            s+=input[j];
        }
        j++;
    }
    return date{data[0], data[1], data[2]};
}

int main(){
    std::string input;
    std::cout << "podaj datę w formacie dd.mm.rrrr\n";
    std::cin >> input;

    date d = parse_date(input);
    std::cout << d.day << " " << miesiac(d.month) << " " << d.year << "\n";
}