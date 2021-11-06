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

        std::cout << std::string(*expr->evaluate()) << "\n";
    }
}

int main() {
    //tokenizePrompt();
    parseMathPrompt();
/*
    std::shared_ptr<Token> op (new Token("+"));
    std::shared_ptr<ValueExpr> a (new ValueExpr(std::shared_ptr<Token>(new Token("5"))));
    std::shared_ptr<ValueExpr> b (new ValueExpr(std::shared_ptr<Token>(new Token("2"))));
    std::shared_ptr<BinaryExpr> expr (new BinaryExpr(a, b, op));
    std::cout << std::string(*expr->evaluate()) << "\n";*/
    return 0;
}