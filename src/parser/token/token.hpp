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
        operator std::string();
        
        Token(TokenType type);
        Token(std::string token);

        TokenType getType();
        int getInt();
        double getDouble();
        bool getBool();
        std::string getString();
        std::string getName();

};

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