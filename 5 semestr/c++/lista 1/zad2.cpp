#include<iostream>
int main() {
    std::string s1 = R"(Instytut Informatyki
Uniwersytetu Wroclawskiego
ul. Joliot-Curie 15
50-383 Wroclaw)",
    s2 = R"(C:\Program Files (x86))",
    // s3 = R"()))(((""""")")""))"; trzeba dać unikalną nazwę ogranicznika
    s3 = R"foo("("()((((()"""")"""""""")()"")))")"))")))foo";
    std::cout << s1 << "\n" << s2 << "\n" << s3;

}