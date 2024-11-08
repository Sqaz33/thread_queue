#include <iostream>
#include <exception>

int main() try {
    std::cout << "Hi from main.cpp\n";
} 
catch(const std::exception& e) { std::cout << e.what() << '\n'; }
catch(...) { std::cout << "!!!\n"; }