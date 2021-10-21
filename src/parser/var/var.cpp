#include "var.hpp"

#include <iostream>

Variable::Variable() {
    activeMember = 'i';
    intValue = 0;
}

Variable::Variable(const Variable& other) {
    activeMember = other.activeMember;
    intValue = other.intValue;
    doubleValue = other.doubleValue;
    boolValue = other.boolValue;
    stringValue = other.stringValue;
}

Variable::Variable(Variable&& other) {
    activeMember = other.activeMember;
    intValue = other.intValue;
    doubleValue = other.doubleValue;
    boolValue = other.boolValue;
    stringValue = other.stringValue;
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

Variable::operator std::string() {
    switch (activeMember) {
        case 'i':
            return std::to_string(intValue);
        case 'd':
            return std::to_string(doubleValue);
        case 'b':
            if (boolValue) return "true";
            else return "false";
        case 's':
            return stringValue;
        default:
            return "INVALID VARIABLE TYPE.";
    }
}

Variable operator + (Variable& a, Variable& b) {
    switch (a.activeMember) {
        case 'i':
            if (b.activeMember == 'i') return Variable(a.intValue + b.intValue);
            else if (b.activeMember == 'd') return Variable(a.intValue + b.doubleValue);
            else goto error;
        case 'd':
            if (b.activeMember == 'i') return Variable(a.doubleValue + b.intValue);
            else if (b.activeMember == 'd') return Variable(a.doubleValue + b.doubleValue);
            else goto error;
        case 'b':
            goto error;
        case 's':
            if (b.activeMember == 's') return Variable(a.stringValue + b.stringValue);
            else goto error;
    }
    error:
        throw OperationTypeException("Invalid types for (+) operator.");
}

Variable operator - (Variable& a, Variable& b) {
    switch (a.activeMember) {
        case 'i':
            if (b.activeMember == 'i') return Variable(a.intValue - b.intValue);
            else if (b.activeMember == 'd') return Variable(a.intValue - b.doubleValue);
            else goto error;
        case 'd':
            if (b.activeMember == 'i') return Variable(a.doubleValue - b.intValue);
            else if (b.activeMember == 'd') return Variable(a.doubleValue - b.doubleValue);
            else goto error;
        case 'b':
        case 's':
            goto error;
    }
    error:
        throw OperationTypeException("Invalid types for (-) operator.");
}

Variable operator * (Variable& a, Variable& b) {
    switch (a.activeMember) {
        case 'i':
            if (b.activeMember == 'i') return Variable(a.intValue * b.intValue);
            else if (b.activeMember == 'd') return Variable(a.intValue * b.doubleValue);
            else goto error;
        case 'd':
            if (b.activeMember == 'i') return Variable(a.doubleValue * b.intValue);
            else if (b.activeMember == 'd') return Variable(a.doubleValue * b.doubleValue);
            else goto error;
        case 'b':
        case 's':
            goto error;
    }
    error:
        throw OperationTypeException("Invalid types for (*) operator.");
}
Variable operator / (Variable& a, Variable& b) {
    switch (a.activeMember) {
        case 'i':
            if (b.activeMember == 'i') return Variable(a.intValue / b.intValue);
            else if (b.activeMember == 'd') return Variable(a.intValue / b.doubleValue);
            else goto error;
        case 'd':
            if (b.activeMember == 'i') return Variable(a.doubleValue / b.intValue);
            else if (b.activeMember == 'd') return Variable(a.doubleValue / b.doubleValue);
            else goto error;
        case 'b':
        case 's':
            goto error;
    }
    error:
        throw OperationTypeException("Invalid types for (/) operator.");
}
Variable operator % (Variable& a, Variable& b) {
        switch (a.activeMember) {
        case 'i':
            if (b.activeMember == 'i') return Variable(a.intValue % b.intValue);
            else goto error;
        case 'd':
        case 'b':
        case 's':
            goto error;
    }
    error:
        throw OperationTypeException("Invalid types for (%) operator.");
}
