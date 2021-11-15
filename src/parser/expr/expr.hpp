#ifndef EXPR_HPP
#define EXPR_HPP

#include <exception>
#include <memory>

#include "../token/token.hpp"
#include "../var/var.hpp"

struct ExpressionEvaluationException : public std::exception {
        std::string message;
        explicit ExpressionEvaluationException (std::string message) { this->message = message; }
        const char *what () { return message.c_str (); }
};

class Expression {
    public:
        virtual ~Expression () = default;
};

class ValueExpr : public Expression {
        friend std::shared_ptr<Variable> evaluate (std::shared_ptr<Expression> expr);

    public:
        ValueExpr (std::shared_ptr<Token> var) { value = var; };

    private:
        std::shared_ptr<Token> value;
};

class IdentExpr : public Expression {
        friend std::shared_ptr<Variable> evaluate (std::shared_ptr<Expression> expr);

    public:
        IdentExpr (std::shared_ptr<Token> id) { name = id; }

    private:
        std::shared_ptr<Token> name;
};

class UnaryExpr : public Expression {
        friend std::shared_ptr<Variable> evaluate (std::shared_ptr<Expression> expr);

    public:
        UnaryExpr (std::shared_ptr<Expression> expr, std::shared_ptr<Token> op) {
            this->expr = expr;
            this->op = op;
        }

    private:
        std::shared_ptr<Expression> expr;
        std::shared_ptr<Token>      op;
};

class BinaryExpr : public Expression {
        friend std::shared_ptr<Variable> evaluate (std::shared_ptr<Expression> expr);

    public:
        BinaryExpr (std::shared_ptr<Expression> left, std::shared_ptr<Expression> right, std::shared_ptr<Token> op) {
            l = left;
            r = right;
            this->op = op;
        };

    private:
        std::shared_ptr<Expression> l;
        std::shared_ptr<Expression> r;
        std::shared_ptr<Token>      op;
};

class GroupExpr : public Expression {
        friend std::shared_ptr<Variable> evaluate (std::shared_ptr<Expression> expr);

    public:
        GroupExpr (std::shared_ptr<Expression> expr) { this->expr = expr; };

    private:
        std::shared_ptr<Expression> expr;
};

std::shared_ptr<Variable> evaluate (std::shared_ptr<Expression> expr);

#endif