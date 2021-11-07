#include <iostream>
#include "parser/token/token.hpp"
#include "parser/token/linepr.hpp"

#include "parser/var/var.hpp"
#include "parser/expr/expr.hpp"

#include "parser/prot/mathparser.hpp"

void tokenizePrompt() {
    while (true) {
        std::string test;
        std::cout << ">> ";
        std::getline(std::cin, test);
        if (test == "") break;
        
        std::vector<Token> tokens = tokenizeString(test);

        for (Token tok : tokens) {
            std::cout << toString(tok) << "\n";
        }
    }
}

void parseMathPrompt() {
    while (true) {
        std::string test;
        std::cout << ">> ";
        std::getline(std::cin, test);
        if (test == "") break;
        
        std::vector<Token> tokens = tokenizeString(test);
        std::shared_ptr<Expression> expr = parseMath(tokens.data());

        try {
            std::cout << std::string(*evaluate(expr)) << "\n";
        }
        catch (OperationTypeException& e) {
            std::cout << e.what() << "\n";
        }
        catch (ExpressionEvaluationException& e) {
            std::cout << e.what() << "\n";
        }
    }
}

int main() {
    //tokenizePrompt();
    parseMathPrompt();

    return 0;
}