#ifndef EXPR_HPP
#define EXPR_HPP

#include <exception>
#include <memory>
#include <vector>

#include "../token/token.hpp"
#include "../type/type.hpp"
#include "../var/var.hpp"

class Interpreter;
struct RuntimeVariable;

typedef std::shared_ptr<RuntimeVariable> RTimeVarSP;

class ExpressionEvaluationException : public std::exception {
    private:
        std::string message;

    public:
        explicit ExpressionEvaluationException(const std::string &message) { this->message = message; }
        const char *what() { return message.c_str(); }
};

class Expression;
typedef std::shared_ptr<Expression> ExprSP;

class Expression {
    public:
        virtual ~Expression() = default;
        RTimeVarSP (Interpreter::*evalFunction)(const ExprSP&) = 0;
};


typedef std::shared_ptr<Variable> VariableSP;

class ValueExpr : public Expression {
    public:
        ValueExpr(const TokenSP &var);
        TokenSP value;
};

typedef std::shared_ptr<ValueExpr> ValueExprSP;

class IdentExpr : public Expression {
    public:
        IdentExpr(const TokenSP &id);
        TokenSP name;
};

typedef std::shared_ptr<IdentExpr> IdentExprSP;

class UnaryExpr : public Expression {
    public:
        UnaryExpr(const ExprSP &expr, const TokenSP &op);
        ExprSP expr;
        TokenSP op;
};

typedef std::shared_ptr<UnaryExpr> UnaryExprSP;

class AsignExpr : public Expression {
    public:
        AsignExpr(const TokenSP &name, const ExprSP &expr);
        TokenSP name;
        ExprSP expr;
};

typedef std::shared_ptr<AsignExpr> AsignExprSP;

class BinaryExpr : public Expression {
    public:
        BinaryExpr(const ExprSP &l, const ExprSP &r, const TokenSP &op);
        ExprSP l;
        ExprSP r;
        TokenSP op;
};

typedef std::shared_ptr<BinaryExpr> BinaryExprSP;

class GroupExpr : public Expression {
    public:
        GroupExpr(const ExprSP &expr);
        ExprSP expr;
};

typedef std::shared_ptr<GroupExpr> GroupExprSP;

class CallExpr : public Expression {
    public:
        CallExpr(const std::vector<ExprSP> &args, const ExprSP &callee);
        std::vector<ExprSP> args;
        ExprSP callee;
};

typedef std::shared_ptr<CallExpr> CallExprSP;

class CastingExpr : public Expression {
    public:
        CastingExpr(const TypeSP &type, const ExprSP &expr);
        TypeSP type;
        ExprSP expr;
};

typedef std::shared_ptr<CastingExpr> CastingExprSP;

class SubscriptExpr : public Expression {
    public:
        SubscriptExpr(const ExprSP &accessed, const ExprSP &index);
        ExprSP accessed;
        ExprSP index;
};

typedef std::shared_ptr<SubscriptExpr> SubscriptExprSP;

VariableSP evaluate(const ExprSP &expr);

#endif