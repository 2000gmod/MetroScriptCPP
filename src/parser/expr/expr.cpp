#include "expr.hpp"

#include "../../util/util.hpp"

using std::shared_ptr;

shared_ptr<Variable> evaluate (shared_ptr<Expression> expr) {
    Expression *ptr = expr.get ();

    if (instanceOf<ValueExpr> (ptr)) {
        ValueExpr           *exprP = (ValueExpr *) ptr;
        shared_ptr<Token>    value = exprP->value;
        shared_ptr<Variable> out (new Variable);

        switch (exprP->value->getType ()) {
            case TokenType::INT_LIT:
                out->activeMember = 'i';
                out->intValue = value->getInt ();
                break;
            case TokenType::DOUBLE_LIT:
                out->activeMember = 'd';
                out->doubleValue = value->getDouble ();
                break;
            case TokenType::BOOL_LIT:
                out->activeMember = 'b';
                out->boolValue = value->getBool ();
                break;
            case TokenType::STRING_LIT:
                out->activeMember = 's';
                out->stringValue = value->getString ();
                break;
            default:
                throw ExpressionEvaluationException ("Unkown type.");
        }
        return out;
    }

    else if (instanceOf<UnaryExpr> (ptr)) {
        UnaryExpr           *exprP = (UnaryExpr *) ptr;
        shared_ptr<Variable> out;
        Variable             minusOne (-1);

        switch (exprP->op->getType ()) {
            case TokenType::PLUS:
                out = evaluate (exprP->expr);
                break;
            case TokenType::MINUS:
                out = shared_ptr<Variable> (new Variable (minusOne * (*evaluate (exprP->expr))));
                break;
            default:
                throw ExpressionEvaluationException ("Invalid unary operator.");
        }
        return out;
    }

    else if (instanceOf<BinaryExpr> (ptr)) {
        shared_ptr<Variable> out;
        BinaryExpr          *exprP = (BinaryExpr *) ptr;

        Variable             leftSide = *(evaluate (exprP->l));
        Variable             rightSide = *(evaluate (exprP->r));
        switch (exprP->op->getType ()) {
            case TokenType::PLUS:
                out = shared_ptr<Variable> (new Variable (leftSide + rightSide));
                break;
            case TokenType::MINUS:
                out = shared_ptr<Variable> (new Variable (leftSide - rightSide));
                break;
            case TokenType::STAR:
                out = shared_ptr<Variable> (new Variable (leftSide * rightSide));
                break;
            case TokenType::SLASH:
                out = shared_ptr<Variable> (new Variable (leftSide / rightSide));
                break;
            case TokenType::MOD:
                out = shared_ptr<Variable> (new Variable (leftSide % rightSide));
                break;
            default:
                throw ExpressionEvaluationException ("Invalid binary operator.");
        }
        return out;
    }

    else if (instanceOf<GroupExpr> (ptr)) {
        GroupExpr *exprP = (GroupExpr *) ptr;
        return shared_ptr<Variable> (new Variable (*evaluate (exprP->expr)));
    }

    else
        throw ExpressionEvaluationException ("Unknown type of expression.");
}