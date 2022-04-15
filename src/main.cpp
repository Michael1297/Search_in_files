#include <iostream>
#include "ConverterJSON.h"

int main() {
    ConverterJSON json;
    try{
        json.search();
    }
    catch (std::exception& e){
        std::cerr << e.what() << "\n";
    }
    catch (...){
        std::cerr << "caught an unknown error\n";
    }
    return 0;
}
