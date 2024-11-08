#include <iostream>
#include <exception>
#include <thread>

int main() try {
    std::cout << "Hi from main.cpp\n";
} 
catch(const std::exception& e) { std::cout << e.what() << '\n'; }
catch(...) { std::cout << "!!!\n"; }