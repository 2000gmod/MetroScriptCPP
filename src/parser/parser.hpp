#ifndef PARSER_HPP
#define PARSER_HPP

#include <memory>
#include <vector>

#include "expr/expr.hpp"
#include "stmt/stmt.hpp"
#include "token/token.hpp"

class ParseException : public std::exception {
    private:
        std::string message;

    public:
        ParseException(std::string message) { this->message = message; }
        const char *what() const throw() { return message.c_str(); }
};

class Parser {
    public:
        Parser(std::vector<Token> tokens);
        std::vector<StmtSP> parse();

    private:
        std::vector<Token> tokens;
        int current = 0;

        bool match(std::vector<TokenType> types);
        Token consume(TokenType type, std::string &message);
        bool check(TokenType type);
        Token advance();
        bool isAtEnd();
        Token peek();
        Token previous();
        ParseException error(const Token &token, std::string &message);
};

#endif