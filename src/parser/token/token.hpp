#ifndef TOKEN_H
#define TOKEN_H

#include <exception>
#include <memory>
#include <ostream>
#include <string>

enum class TokenType {
    PLUS,
    MINUS,
    STAR,
    SLASH,
    COMMA,
    DOT,
    MOD,
    INC,
    DEC,

    ASSIGN,
    EQ,
    NOT_EQ,
    LT,
    GT,
    LEQ,
    GEQ,
    SEMICOLON,

    AND,
    NOT,
    OR,

    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_SQR,
    RIGHT_SQR,
    LEFT_CUR,
    RIGHT_CUR,

    IF,
    ELSE,
    FOR,
    WHILE,
    BREAK,
    CONTINUE,

    INT_LIT,
    DOUBLE_LIT,
    BOOL_LIT,
    STRING_LIT,
    IDENTIFIER,
    TYPE,

    RETURN,
    ERROR,
    EOFILE
};

std::string toString(const TokenType &tp);

class Token {
    private:
        TokenType type;
        int intValue;
        double doubleValue;
        bool boolValue;
        std::string stringValue;

        std::string identifierName;
        std::string typeString;

        int line;

        void formatEscapeSeqs();

    public:
        Token(TokenType type);
        Token(std::string token);
        Token(std::string token, int line);
        Token(const Token &other);

        TokenType getType() const;
        int getInt() const;
        double getDouble() const;
        bool getBool() const;
        std::string getString() const;
        std::string getName() const;
        std::string getTypeName() const;

        bool isLiteral() const;
        bool isOperator() const;
        bool isBasicType(std::string subToken);

        int getLine() const;

        void nullify();
};
typedef std::shared_ptr<Token> TokenSP;

std::string toString(const Token &tk);

class NumberFormatException : public std::exception {
    private:
        std::string message;

    public:
        NumberFormatException(std::string message) { this->message = message; }
        const char *what() const throw() { return message.c_str(); }
};

std::ostream &operator<<(std::ostream &, Token &);

const std::string keywords[] = {"for", "while",  "if",    "else",     "and",  "or",
                                "not", "return", "break", "continue", "true", "false"};

#endif