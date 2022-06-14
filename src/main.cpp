#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "info.hpp"
#include "parser/expr/expr.hpp"
#include "parser/parser.hpp"
#include "parser/prot/mathparser.hpp"
#include "parser/token/linepr.hpp"
#include "parser/token/token.hpp"
#include "parser/var/var.hpp"
#include "interpreter/interpreter.hpp"

void tokenizePrompt() {
    std::cout << "\n# CLI String Tokenizer mode\n";
    while (true) {
        std::string test;
        std::cout << ">> ";
        std::getline(std::cin, test);
        if (test == "") break;

        std::vector<Token> tokens = tokenizeString(test);

        for (Token &tok : tokens) {
            std::cout << toString(tok) << "\n";
        }
    }
}

void parseMathPrompt() {
    std::cout << "\n# CLI String math parser mode\n";
    while (true) {
        std::string test;
        std::cout << ">> ";
        std::getline(std::cin, test);
        if (test == "") break;

        std::vector<Token> tokens = tokenizeString(test);
        ExprSP expr = parseMath(tokens.data());

        try {
            std::cout << std::string(*evaluate(expr)) << "\n";
        } catch (OperationTypeException &e) {
            std::cout << e.what() << "\n";
        } catch (ExpressionEvaluationException &e) {
            std::cout << e.what() << "\n";
        }
    }
}

void runFile(const std::string &filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    if (!(buffer << file.rdbuf())) {
        std::cout << "FATAL ERROR: file \"" << filename << "\" cannot be opened.\n";
        exit(EXIT_FAILURE);
    }
    file.close();
    std::vector<std::string> lines = getFileLines(buffer.str());

    std::vector<Token> tokens = tokenizeString(buffer.str());

    try {
        Parser parser(tokens, lines);
        std::vector<StmtSP> statements = parser.parse();

        Interpreter inter(statements);
        inter.load();
        inter.run();
        
    } 
    catch (ParseException &) {
        exit(1);
    }
    catch (RuntimeException &e) {
        std::cout << e.what() << "\n";
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv) {
    if (argc == 1) {
        std::cout << "FATAL ERROR: No input file or CLI execution option given.\n";
        return EXIT_FAILURE;
    }

    if (argc == 2) {
        std::string secondArg(argv[1]);
        if (secondArg == "-t") {
            printInfo();
            tokenizePrompt();
            return EXIT_SUCCESS;
        }
        if (secondArg == "-m") {
            printInfo();
            parseMathPrompt();
            return EXIT_SUCCESS;
        }

        else {
            runFile(secondArg);
        }
    }

    return EXIT_SUCCESS;
}