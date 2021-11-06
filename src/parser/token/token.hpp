#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <ostream>
#include <exception>


enum class TokenType {
    PLUS, MINUS, STAR, SLASH,
    COMMA, DOT, MOD,

    ASSIGN, EQ, NOT_EQ,
    LT, GT, LEQ, GEQ, SEMICOLON,

    AND, NOT, OR,

    LEFT_PAREN, RIGHT_PAREN, LEFT_SQR, RIGHT_SQR, LEFT_CUR, RIGHT_CUR,

    IF, ELSE, FOR, WHILE, BREAK, CONTINUE,


    INT, DOUBLE, BOOL, STRING,

    INT_LIT, DOUBLE_LIT, BOOL_LIT, STRING_LIT,
    IDENTIFIER,

    RETURN,
    ERROR, EOFILE
};

std::string toString(const TokenType& tp);


class Token {
    private:
        TokenType type;
        int intValue;
        double doubleValue;
        bool boolValue;
        std::string stringValue;

        std::string identifierName;

    public:
        
        
        Token(TokenType type);
        Token(std::string token);
        Token(const Token& other);

        TokenType getType() const;
        int getInt() const;
        double getDouble() const;
        bool getBool() const;
        std::string getString() const;
        std::string getName() const;

        bool isLiteral() const;
        bool isOperator() const;

        void nullify();

};

std::string toString(const Token& tk);

struct NumberFormatException : public std::exception {
    std::string message;
    NumberFormatException(std::string message) {this->message = message;}
    const char *what() const throw() {
        return message.c_str();
    }
};

std::ostream& operator << (std::ostream&, Token&);

const std::string keywords[] = {
    "for", "while", "if", "else",
    "and", "or", "not", "return",
    "break", "continue",
    "int", "double", "bool", "string",
    "true", "false"
};


#endif