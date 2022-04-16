#include <iostream>
#include "Search.hpp"

int main() {
    try{
        search();
    }
    catch (std::exception& e){
        std::cerr << e.what() << "\n";
    }
    catch (...){
        std::cerr << "caught an unknown error\n";
    }
    return 0;
}
