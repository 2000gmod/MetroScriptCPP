#ifndef VAR_HPP
#define VAR_HPP

#include <string>
#include <exception>

#include "../token/token.hpp"

struct OperationTypeException : public std::exception {
    std::string message;
    explicit OperationTypeException(const std::string& message) {
        this->message = std::string("OperationTypeException: ") + message;
    }
    const char *what() {
        return message.c_str();
    }
};

struct Variable {
    int intValue;
    double doubleValue;
    bool boolValue;
    std::string stringValue;

    char activeMember;

    Variable();
    Variable(const Variable& other);
    Variable(Variable&& other);
    Variable(Token& token);
    Variable(int intValue);
    Variable(double doubleValue);
    Variable(bool boolValue);
    Variable(std::string stringValue);

    operator std::string();
};


Variable operator + (Variable& a, Variable& b);
Variable operator - (Variable& a, Variable& b);
Variable operator * (Variable& a, Variable& b);
Variable operator / (Variable& a, Variable& b);
Variable operator % (Variable& a, Variable& b);

#endif