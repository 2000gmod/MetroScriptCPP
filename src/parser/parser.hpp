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

        StmtSP declaration();
        StmtSP statement();
        StmtSP exprStatement();
        StmtSP forStatement();
        StmtSP whileStatement();
        StmtSP function();
        StmtSP ifStatement();
        StmtSP returnStatement();
        std::vector<StmtSP> block();
        StmtSP varDeclaration();

        ExprSP expression();
        ExprSP assignment();
        ExprSP orExpr();
        ExprSP andExpr();
        ExprSP equal();
        ExprSP comparison();
        ExprSP addition();
        ExprSP multiplication();
        ExprSP casting();
        ExprSP unary();
        ExprSP call();
        ExprSP finishCall(ExprSP callee);
        ExprSP primary();

        bool match(TokenType type);
        bool matchAny(std::vector<TokenType> types);

        TokenSP consume(TokenType type, std::string message);
        TokenSP consumeAny(std::vector<TokenType> types, std::string message);

        bool check(TokenType type);
        bool checkAny(std::vector<TokenType> types);

        TokenSP advance();
        TokenSP goBack();
        bool isAtEnd();
        TokenSP peek();
        TokenSP previous();
        ParseException error(TokenSP token, std::string message);
        void synchronize();
};

#endif