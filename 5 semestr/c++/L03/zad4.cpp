#include <iostream>

template<typename T, typename Y>
void move_pointer(T a, Y &b, std::true_type){
    b = *a;
}
template<typename T, typename Y>
void move_no_pointer(T a, Y &b, std::true_type){
    b = a;
}

template<typename T, typename Y>
void move_check(T *a, Y &b, std::true_type){
    move_pointer(a, b, std::is_convertible<T, Y>());
}
template<typename T, typename Y>
void move_check(T a, Y &b, std::false_type){
    move_no_pointer(a, b, std::is_convertible<T, Y>());
}

template<typename T, typename Y>
void move(T a, Y &b){
    move_check(a, b, std::is_pointer<T>());
}
int main(){

    int a = 10;
    long long int b = 100;
    int c = 999;
    std::string s = "foo";
    move(a, b);
    std::cout << b << "\n";
    b = 1001;
    move(&a, b);
    move(&a, c);
    std::cout << b << "\n";
    std::cout << c << "\n";
    // move(a, s);
}