#include <iostream>
#include <ratio>
constexpr intmax_t end{46}; // wyrzuca blad przy 47
template<typename R, intmax_t n = 1>
void sum_of_harmonic_sequence(){
    typedef std::ratio_add<R, std::ratio<1, (intmax_t) (n+1)> > Harmonic;
    std::cout << Harmonic::num << " / " << Harmonic::den << "\n";
}
template<typename R>
void calculate(std::integral_constant<intmax_t, end>)
{
}

template <typename R, intmax_t n = 0>
void calculate(std::integral_constant<intmax_t, n> = std::integral_constant<intmax_t, 0>()){
    sum_of_harmonic_sequence<R, n>();
    calculate<std::ratio_add<R, std::ratio<1, (intmax_t)(n+1)> >> (std::integral_constant<intmax_t, n + 1>());
}
int main()
{
    typedef std::ratio<1, 1> Harmonic1;
    typedef std::ratio_add<Harmonic1, std::ratio<1, 2> > Harmonic2;
    typedef std::ratio_add<Harmonic2, std::ratio<1, 3> > Harmonic3;
    //itd
    calculate<std::ratio<0, 1>>();
}