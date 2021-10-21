#include <iostream>
#include "parser/token/token.hpp"
#include "parser/token/linepr.hpp"

#include "parser/var/var.hpp"
#include "parser/expr/expr.hpp"

void tokenizePrompt() {
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
}

int main() {
    //tokenizePrompt();
    try {
        std::shared_ptr<Variable> test;

        std::shared_ptr<Token> a(new Token("\"aaa\""));
        std::shared_ptr<Token> b(new Token("\"hola\""));
        std::shared_ptr<Token> op (new Token("+"));

        std::shared_ptr<ValueExpr> left (new ValueExpr(a));
        std::shared_ptr<ValueExpr> right (new ValueExpr(b));


        test = (BinaryExpr(left, right, op)).evaluate();
        std::cout << std::string(*test) << "\n";
    }
    catch (ExpressionEvaluationException& e) {
        std::cout << e.what() << "\n";
    }
    catch (OperationTypeException& e) {
        std::cout << e.what() << "\n";
    }

    return 0;
}