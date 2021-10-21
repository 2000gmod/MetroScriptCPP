#include "expr.hpp"

using std::shared_ptr;

shared_ptr<Variable> ValueExpr::evaluate() {
    shared_ptr<Variable> out(new Variable);

    switch (value->getType()) {
        case TokenType::INT_LIT:
            out->activeMember = 'i';
            out->intValue = value->getInt();
            break;
        case TokenType::DOUBLE_LIT:
            out->activeMember = 'd';
            out->doubleValue = value->getDouble();
            break;
        case TokenType::BOOL_LIT:
            out->activeMember = 'b';
            out->boolValue = value->getBool();
            break;
        case TokenType::STRING_LIT:
            out->activeMember = 's';
            out->stringValue = value->getString();
            break;
        default:
            throw ExpressionEvaluationException("Unkown type.");
    }
    return out;
}

shared_ptr<Variable> UnaryExpr::evaluate() {
    shared_ptr<Variable> out;
    Variable minusOne(-1);
    switch(op->getType()) {
        case TokenType::PLUS:
            out = expr->evaluate();
            break;
        case TokenType::MINUS:
            out = shared_ptr<Variable>(new Variable(minusOne * (*expr->evaluate())));
            break;
        default:
            throw ExpressionEvaluationException("Invalid unary operator.");
    }
    return out;
}

shared_ptr<Variable> BinaryExpr::evaluate() {
    shared_ptr<Variable> out;
    Variable leftSide = *(l->evaluate());
    Variable rightSide = *(r->evaluate());
    switch(op->getType()) {
        case TokenType::PLUS:
            out = shared_ptr<Variable> (new Variable(leftSide + rightSide));
            break;
        case TokenType::MINUS:
            out = shared_ptr<Variable> (new Variable((*(l->evaluate()) - *(r->evaluate()))));
            break;
        case TokenType::STAR:
            out = shared_ptr<Variable> (new Variable((*(l->evaluate()) * *(r->evaluate()))));
            break;
        case TokenType::SLASH:
            out = shared_ptr<Variable> (new Variable((*(l->evaluate()) / *(r->evaluate()))));
            break;
        case TokenType::MOD:
            out = shared_ptr<Variable> (new Variable((*(l->evaluate()) % *(r->evaluate()))));
            break;
        default:
            throw ExpressionEvaluationException("Invalid binary operator.");
    }
    return out;
}