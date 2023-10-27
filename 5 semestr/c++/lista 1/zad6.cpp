#include <iostream>
#include <cmath>
#include <complex>
const double epsilon = 0.0005;
double delta(double a, double b, double c){
    return b*b - 4.0*a*c; 
}

void solve(double a, double b, double c){
    if(double d = delta(a, b, c); abs(d) < epsilon){
        double x = -b/(2.0*a);
        std::cout << "jedno miejsce zerowe -> "  << x << "\n";
    }else if(d > 0){
        double x1 = (-b + std::sqrt(d))/(2.0*a);
        double x2 = (-b - std::sqrt(d))/(2.0*a);
        std::cout << "dwa miejsca zerowe -> "  << x1 << " " << x2 << "\n";
    }else{
        double r = -b/(2*a);
        double i =sqrt(-d)/(2*a);
        std::cout << "Dwa miejsca zerowe urojone -> "  << r << "+" << i <<"i " << r << "-" << i <<"i\n"; 
}

}
// g++ -std=gnu++17 zad6.cpp

int main(){
    solve(1, 4, -10);
    solve(1, 4, 5);
    solve(1, -4, 4);
}