#ifndef PARSER_HPP
#define PARSER_HPP

#include "expr/expr.hpp"
#include "stmt/stmt.hpp"
#include "token/token.hpp"
#include "type/type.hpp"

#include <initializer_list>

class ParseException : public std::exception {
    private:
        std::string message;

    public:
        ParseException(std::string message) { this->message = message; }
        const char *what() const throw() { return message.c_str(); }
};

class Parser {
    public:
        Parser(const std::vector<Token> &tokens, const std::vector<std::string> &fileLines);
        std::vector<StmtSP> parse();

    private:
        std::vector<Token> tokens;
        std::vector<std::string> fileLines;
        int current = 0;

        bool isDeclaration();
        StmtSP declaration();
        VarDeclStmtSP varDeclaration(TypeSP type, const Token &name);
        FunctionDeclStmtSP functionDecl(TypeSP type, const Token &name);

        StmtSP statement();

        StmtSP expressionStatement();
        StmtSP forStatement();
        StmtSP ifStatement();
        StmtSP returnStatement();
        StmtSP blockStatement();
        StmtSP whileStatement();
        StmtSP breakStatement();
        StmtSP continueStatement();

        ExprSP expression();
        ExprSP assignment();
        ExprSP orExpr();
        ExprSP andExpr();
        ExprSP eqExpr();
        ExprSP compExpr();
        ExprSP addition();
        ExprSP multiplication();
        ExprSP unary();
        ExprSP casting();
        ExprSP subscript();
        ExprSP callExpr();
        ExprSP finishCallExpr(ExprSP callee);
        ExprSP primaryExpr();

        TypeSP type();
        TypeSP functionPointerType();
        TypeSP arrayType();
        TypeSP basicType();

        bool match(const std::initializer_list<TokenType> types);
        bool match(TokenType type);
        const Token &consume(TokenType type, const char *message);
        bool check(TokenType type);
        const Token &advance();
        bool isAtEnd();
        const Token &peek();
        const Token &previous();
        ParseException error(const Token &token, const char *message);
        void reportError(int line, const std::string &message);
};

#endif