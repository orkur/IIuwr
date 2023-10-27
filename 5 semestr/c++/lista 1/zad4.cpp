#include <iostream>
#include <cstdint>
enum class name: std::uint16_t{
    Kuba,
    Jakub,
    Amelia,
    Pawel,
    Mirek
};
void Hello(const std::string &str, name person){

    const auto convert_name = [](name person) -> std::string {
        switch(person){
            case name::Kuba:     return "Kuba!";
            case name::Jakub:    return "Jakub!";
            case name::Amelia:   return "Amelia!";
            case name::Pawel:    return "Pawel!";
            case name::Mirek:    return "Mirek!";
            default:             return "Ty!";
        }
    };
    std::cout << str << " " << convert_name(person) << std::endl;
}

int main()
{
    Hello("Hejka ", name::Amelia);
}