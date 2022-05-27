#include "expr.hpp"

#include <memory>

#include "../../util/util.hpp"
#include "../../interpreter/interpreter.hpp"

VariableSP evaluate(const ExprSP &expr) {
    Expression *ptr = expr.get();

    if (instanceOf<ValueExpr>(ptr)) {
        auto *exprP = (ValueExpr *) ptr;
        auto value = exprP->value;
        VariableSP out(new Variable);

        switch (exprP->value->getType()) {
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

    else if (instanceOf<UnaryExpr>(ptr)) {
        auto *exprP = (UnaryExpr *) ptr;
        VariableSP out;
        Variable minusOne(-1);

        switch (exprP->op->getType()) {
            case TokenType::PLUS:
                out = evaluate(exprP->expr);
                break;
            case TokenType::MINUS:
                out = std::make_shared<Variable>(Variable(minusOne * (*evaluate(exprP->expr))));
                break;
            default:
                throw ExpressionEvaluationException("Invalid unary operator.");
        }
        return out;
    }

    else if (instanceOf<BinaryExpr>(ptr)) {
        VariableSP out;
        auto *exprP = (BinaryExpr *) ptr;

        Variable leftSide = *(evaluate(exprP->l));
        Variable rightSide = *(evaluate(exprP->r));
        switch (exprP->op->getType()) {
            case TokenType::PLUS:
                out = std::make_shared<Variable>(Variable(leftSide + rightSide));
                break;
            case TokenType::MINUS:
                out = std::make_shared<Variable>(Variable(leftSide - rightSide));
                break;
            case TokenType::STAR:
                out = std::make_shared<Variable>(Variable(leftSide * rightSide));
                break;
            case TokenType::SLASH:
                out = std::make_shared<Variable>(Variable(leftSide / rightSide));
                break;
            case TokenType::MOD:
                out = std::make_shared<Variable>(Variable(leftSide % rightSide));
                break;
            default:
                throw ExpressionEvaluationException("Invalid binary operator.");
        }
        return out;
    }

    else if (instanceOf<GroupExpr>(ptr)) {
        GroupExpr *exprP = (GroupExpr *) ptr;
        return std::make_shared<Variable>(Variable(*evaluate(exprP->expr)));
    }

    else
        throw ExpressionEvaluationException("Unknown type of expression.");
}

ValueExpr::ValueExpr(const TokenSP &var) { 
    value = var;
    evalFunction = &Interpreter::evalValueExpr;
}

IdentExpr::IdentExpr(const TokenSP &id) { 
    name = id; 
    evalFunction = &Interpreter::evalIdentExpr;
}

UnaryExpr::UnaryExpr(const ExprSP &expr, const TokenSP &op) {
    this->expr = expr;
    this->op = op;
    evalFunction = &Interpreter::evalUnaryExpr;
}

AsignExpr::AsignExpr(const TokenSP &name, const ExprSP &expr) {
    this->name = name;
    this->expr = expr;
    evalFunction = &Interpreter::evalAsignExpr;
}

BinaryExpr::BinaryExpr(const ExprSP &l, const ExprSP &r, const TokenSP &op) {
    this->l = l;
    this->r = r;
    this->op = op;
    evalFunction = &Interpreter::evalBinaryExpr;
}

GroupExpr::GroupExpr(const ExprSP &expr) { 
    this->expr = expr;
    evalFunction = &Interpreter::evalGroupExpr;
}

CallExpr::CallExpr(const std::vector<ExprSP> &args, const ExprSP &callee) {
    this->args = args;
    this->callee = callee;
    evalFunction = &Interpreter::evalCallExpr;
}

CastingExpr::CastingExpr(const TypeSP &type, const ExprSP &expr) {
    this->type = type;
    this->expr = expr;
    evalFunction = &Interpreter::evalCastingExpr;
}

SubscriptExpr::SubscriptExpr(const ExprSP &accessed, const ExprSP &index) {
    this->accessed = accessed;
    this->index = index;
    evalFunction = &Interpreter::evalSubscriptExpr;
}