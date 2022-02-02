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
        VarDeclStmtSP varDeclaration(Token type, Token name);
        FunctionDeclStmtSP functionDecl(Token type, Token name);

        StmtSP statement();

        StmtSP expressionStatement();
        StmtSP forStatement();
        StmtSP ifStatement();
        StmtSP returnStatement();
        StmtSP blockStatement();
        StmtSP whileStatement();

        ExprSP expression();
        ExprSP assignment();
        ExprSP orExpr();
        ExprSP andExpr();
        ExprSP eqExpr();
        ExprSP compExpr();
        ExprSP addition();
        ExprSP multiplication();
        ExprSP unary();
        ExprSP callExpr();
        ExprSP finishCallExpr(ExprSP callee);
        ExprSP primaryExpr();
        
        bool match(std::vector<TokenType> types);
        bool match(TokenType type);
        const Token& consume(TokenType type, const char *message);
        bool check(TokenType type);
        const Token &advance();
        bool isAtEnd();
        const Token &peek();
        const Token &previous();
        ParseException error(const Token &token, const char *message);
};

#endif