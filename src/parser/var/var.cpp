#include "var.hpp"

#include <iostream>

Variable::Variable() {
    activeMember = 'i';
    intValue = 0;
}

Variable::Variable(const Variable &other) {
    activeMember = other.activeMember;
    intValue = other.intValue;
    doubleValue = other.doubleValue;
    boolValue = other.boolValue;
    stringValue = other.stringValue;
}

Variable::Variable(Variable &&other) {
    activeMember = other.activeMember;
    intValue = other.intValue;
    doubleValue = other.doubleValue;
    boolValue = other.boolValue;
    stringValue = other.stringValue;
}

Variable::Variable(Token &token) {
    if (!token.isLiteral()) throw std::exception();

    switch (token.getType()) {
        case TokenType::INT_LIT:
            activeMember = 'i';
            intValue = token.getInt();
            return;
        case TokenType::DOUBLE_LIT:
            activeMember = 'd';
            doubleValue = token.getDouble();
            return;
        case TokenType::BOOL_LIT:
            activeMember = 'b';
            boolValue = token.getBool();
            return;
        case TokenType::STRING_LIT:
            activeMember = 's';
            stringValue = token.getString();
            return;
        default:
            throw std::exception();
    }
}

Variable::Variable(int intValue) {
    activeMember = 'i';
    this->intValue = intValue;
}

Variable::Variable(double doubleValue) {
    activeMember = 'd';
    this->doubleValue = doubleValue;
}

Variable::Variable(bool boolValue) {
    activeMember = 'b';
    this->boolValue = boolValue;
}

Variable::Variable(std::string stringValue) {
    activeMember = 's';
    this->stringValue = stringValue;
}

Variable::operator std::string() const {
    switch (activeMember) {
        case 'i':
            return std::to_string(intValue);
        case 'd':
            return std::to_string(doubleValue);
        case 'b':
            if (boolValue)
                return "true";
            else
                return "false";
        case 's':
            return stringValue;
        default:
            return "INVALID VARIABLE TYPE.";
    }
}

Variable operator+(const Variable &a, const Variable &b) {
    switch (a.activeMember) {
        case 'i':
            if (b.activeMember == 'i')
                return Variable(a.intValue + b.intValue);
            else if (b.activeMember == 'd')
                return Variable(a.intValue + b.doubleValue);
            else
                break;
        case 'd':
            if (b.activeMember == 'i')
                return Variable(a.doubleValue + b.intValue);
            else if (b.activeMember == 'd')
                return Variable(a.doubleValue + b.doubleValue);
            else
                break;
        case 'b':
            break;
        case 's':
            if (b.activeMember == 's')
                return Variable(a.stringValue + b.stringValue);
            else
                break;
    }
    throw OperationTypeException("Invalid types for (+) operator.");
}

Variable operator-(const Variable &a, const Variable &b) {
    switch (a.activeMember) {
        case 'i':
            if (b.activeMember == 'i')
                return Variable(a.intValue - b.intValue);
            else if (b.activeMember == 'd')
                return Variable(a.intValue - b.doubleValue);
            else
                break;
        case 'd':
            if (b.activeMember == 'i')
                return Variable(a.doubleValue - b.intValue);
            else if (b.activeMember == 'd')
                return Variable(a.doubleValue - b.doubleValue);
            else
                break;
        case 'b':
        case 's':
            break;
    }
    throw OperationTypeException("Invalid types for (-) operator.");
}

Variable operator*(const Variable &a, const Variable &b) {
    switch (a.activeMember) {
        case 'i':
            if (b.activeMember == 'i')
                return Variable(a.intValue * b.intValue);
            else if (b.activeMember == 'd')
                return Variable(a.intValue * b.doubleValue);
            else
                break;
        case 'd':
            if (b.activeMember == 'i')
                return Variable(a.doubleValue * b.intValue);
            else if (b.activeMember == 'd')
                return Variable(a.doubleValue * b.doubleValue);
            else
                break;
        case 'b':
        case 's':
            break;
    }
    throw OperationTypeException("Invalid types for (*) operator.");
}
Variable operator/(const Variable &a, const Variable &b) {
    switch (a.activeMember) {
        case 'i':
            if (b.activeMember == 'i')
                return Variable(a.intValue / b.intValue);
            else if (b.activeMember == 'd')
                return Variable(a.intValue / b.doubleValue);
            else
                break;
        case 'd':
            if (b.activeMember == 'i')
                return Variable(a.doubleValue / b.intValue);
            else if (b.activeMember == 'd')
                return Variable(a.doubleValue / b.doubleValue);
            else
                break;
        case 'b':
        case 's':
            break;
    }
    throw OperationTypeException("Invalid types for (/) operator.");
}
Variable operator%(const Variable &a, const Variable &b) {
    switch (a.activeMember) {
        case 'i':
            if (b.activeMember == 'i')
                return Variable(a.intValue % b.intValue);
            else
                break;
        case 'd':
        case 'b':
        case 's':
            break;
    }
    throw OperationTypeException("Invalid types for (%) operator.");
}

bool operator ==(const Variable &a, const Variable &b) {
    if (a.activeMember != b.activeMember) throw OperationTypeException("Comparing different types.");
    switch (a.activeMember) {
        case 'i':
            return a.intValue == b.intValue;
        case 'd':
            return a.doubleValue == b.doubleValue;
        case 's':
            return a.stringValue == b.stringValue;
        case 'b':
            return a.boolValue == b.boolValue;
    }
    return false;
}

bool operator !=(const Variable &a, const Variable &b) {
    return !(a == b);
}

bool operator <(const Variable &a, const Variable &b) {
    if (!((a.activeMember == 'i' || a.activeMember == 'd') && (b.activeMember == 'i' || b.activeMember == 'd'))) {
        throw OperationTypeException("Invalid types for relational operator");
    }
    char t1 = a.activeMember;
    char t2 = b.activeMember;

    if (t1 == 'i' && t2 == 'i') return a.intValue < b.intValue;
    if (t1 == 'i' && t2 == 'd') return a.intValue < b.doubleValue;
    if (t1 == 'd' && t2 == 'i') return a.doubleValue < b.intValue;
    if (t1 == 'd' && t2 == 'd') return a.doubleValue < b.doubleValue;
    return false;
}

bool operator >(const Variable &a, const Variable &b) {
    return (!(a < b) && a != b);
}

bool operator >=(const Variable &a, const Variable &b) {
    return !(a < b);
}

bool operator <=(const Variable &a, const Variable &b) {
    return !(a > b);
}

bool operator ||(const Variable &a, const Variable &b) {
    if (a.activeMember != 'b' || b.activeMember != 'b') {
        throw OperationTypeException("Can only OR bool types.");
    }
    return a.boolValue || b.boolValue;
}

bool operator &&(const Variable &a, const Variable &b) {
    if (a.activeMember != 'b' || b.activeMember != 'b') {
        throw OperationTypeException("Can only AND bool types.");
    }
    return a.boolValue && b.boolValue;
}
