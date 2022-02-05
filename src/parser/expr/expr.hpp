#ifndef EXPR_HPP
#define EXPR_HPP

#include <exception>
#include <memory>
#include <vector>

#include "../token/token.hpp"
#include "../var/var.hpp"
#include "../type/type.hpp"

class ExpressionEvaluationException : public std::exception {
    private:
        std::string message;

    public:
        explicit ExpressionEvaluationException(std::string message) { this->message = message; }
        const char *what() { return message.c_str(); }
};

class Expression {
    public:
        virtual ~Expression() = default;
};

typedef std::shared_ptr<Expression> ExprSP;
typedef std::shared_ptr<Variable> VariableSP;

class ValueExpr : public Expression {
    public:
        ValueExpr(TokenSP var) { value = var; };
        TokenSP value;
};

typedef std::shared_ptr<ValueExpr> ValueExprSP;

class IdentExpr : public Expression {
    public:
        IdentExpr(TokenSP id) { name = id; }
        TokenSP name;
};

typedef std::shared_ptr<IdentExpr> IdentExprSP;

class UnaryExpr : public Expression {
    public:
        UnaryExpr(ExprSP expr, TokenSP op) {
            this->expr = expr;
            this->op = op;
        }
        ExprSP expr;
        TokenSP op;
};

typedef std::shared_ptr<UnaryExpr> UnaryExprSP;

class AsignExpr : public Expression {
    public:
        AsignExpr(TokenSP name, ExprSP expr) {
            this->name = name;
            this->expr = expr;
        }
        TokenSP name;
        ExprSP expr;
};

typedef std::shared_ptr<AsignExpr> AsignExprSP;

class BinaryExpr : public Expression {
    public:
        BinaryExpr(ExprSP l, ExprSP r, TokenSP op) {
            this->l = l;
            this->r = r;
            this->op = op;
        };
        ExprSP l;
        ExprSP r;
        TokenSP op;
};

typedef std::shared_ptr<BinaryExpr> BinaryExprSP;

class GroupExpr : public Expression {
    public:
        GroupExpr(ExprSP expr) { this->expr = expr; };
        ExprSP expr;
};

typedef std::shared_ptr<GroupExpr> GroupExprSP;

class CallExpr : public Expression {
    public:
        CallExpr(std::vector<ExprSP> args, ExprSP callee) {
            this->args = args;
            this->callee = callee;
        }
        std::vector<ExprSP> args;
        ExprSP callee;
};

typedef std::shared_ptr<CallExpr> CallExprSP;

class CastingExpr : public Expression {
    public:
        CastingExpr(TypeSP type, ExprSP expr) {
            this->type = type;
            this->expr = expr;
        }
        TypeSP type;
        ExprSP expr;
};

typedef std::shared_ptr<CastingExpr> CastingExprSP;

class SubscriptExpr : public Expression {
    public:
        SubscriptExpr(ExprSP accessed, ExprSP index) {
            this->accessed = accessed;
            this->index = index;
        }
        ExprSP accessed;
        ExprSP index;
};

typedef std::shared_ptr<SubscriptExpr> SubscriptExprSP;

VariableSP evaluate(ExprSP expr);

#endif