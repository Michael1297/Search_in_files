#include <iostream>
#include "ConverterJSON.h"

int main() {
    ConverterJSON JSON;
    for(auto& i : JSON.GetTextDocuments())  std::cout << i << std::endl;
    std::cout << JSON.GetResponsesLimit() << std::endl;
    return 0;
}
