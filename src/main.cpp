#include <iostream>
#include "token/token.hpp"
#include "token/linepr.hpp"

int main() {
    while (true) {
        std::string test;
        std::cout << ">> ";
        std::getline(std::cin, test);
        if (test == "") break;
        
        std::vector<Token> tokens = tokenizeString(test);

        for (Token tok : tokens) {
            std::cout << std::string(tok) << "\n";
        }
    }
    return 0;
}