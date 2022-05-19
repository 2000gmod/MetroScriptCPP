#ifndef VAR_HPP
#define VAR_HPP

#include <exception>
#include <string>

#include "../token/token.hpp"

class OperationTypeException : public std::exception {
    private:
        std::string message;

    public:
        explicit OperationTypeException(const std::string &message) {
            this->message = std::string("OperationTypeException: ") + message;
        }
        const char *what() { return message.c_str(); }
};

class Variable {
    public:
        int intValue;
        double doubleValue;
        bool boolValue;
        std::string stringValue;

        char activeMember;

        Variable();
        Variable(const Variable &other);
        Variable(Variable &&other);
        Variable(Token &token);
        Variable(int intValue);
        Variable(double doubleValue);
        Variable(bool boolValue);
        Variable(std::string stringValue);
        Variable &operator =(const Variable &) = default;

        operator std::string();
};

Variable operator+(const Variable &a, const Variable &b);
Variable operator-(const Variable &a, const Variable &b);
Variable operator*(const Variable &a, const Variable &b);
Variable operator/(const Variable &a, const Variable &b);
Variable operator%(const Variable &a, const Variable &b);

bool operator ==(const Variable &a, const Variable &b);
bool operator !=(const Variable &a, const Variable &b);
bool operator <(const Variable &a, const Variable &b);
bool operator >(const Variable &a, const Variable &b);
bool operator >=(const Variable &a, const Variable &b);
bool operator <=(const Variable &a, const Variable &b);

bool operator ||(const Variable &a, const Variable &b);
bool operator &&(const Variable &a, const Variable &b);



#endif