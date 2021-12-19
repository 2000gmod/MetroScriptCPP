#include "expr.hpp"

#include <memory>

#include "../../util/util.hpp"

VariableSP evaluate(ExprSP expr) {
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