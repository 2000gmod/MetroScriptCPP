#ifndef STMT_HPP
#define STMT_HPP

#include <memory>
#include <tuple>
#include <vector>

#include "../expr/expr.hpp"
#include "../token/token.hpp"

class Statement {
    public:
        virtual ~Statement() = default;
};

typedef std::shared_ptr<Statement> StmtSP;

class BlockStmt : public Statement {
    public:
        BlockStmt(std::vector<StmtSP> statements) { this->statements = statements; }
        std::vector<StmtSP> statements;
};

typedef std::shared_ptr<BlockStmt> BlockStmtSP;

class ExprStmt : public Statement {
    public:
        ExprStmt(ExprSP expr) { this->expr = expr; }
        ExprSP expr;
};

typedef std::shared_ptr<ExprStmt> ExprStmtSP;

typedef std::tuple<TypeSP, TokenSP> parameterT;
class FunctionDeclStmt : public Statement {
    public:
        FunctionDeclStmt(TypeSP type, TokenSP name, std::vector<parameterT> parameters, StmtSP body) {
            this->type = type;
            this->name = name;
            this->parameters = parameters;
            this->body = body;
        }
        TypeSP type;
        TokenSP name;
        std::vector<parameterT> parameters;
        StmtSP body;
};

typedef std::shared_ptr<FunctionDeclStmt> FunctionDeclStmtSP;

class ReturnStmt : public Statement {
    public:
        ReturnStmt(ExprSP value) { this->value = value; }
        ExprSP value;
};

typedef std::shared_ptr<ReturnStmt> ReturnStmtSP;

class VarDeclStmt : public Statement {
    public:
        VarDeclStmt(TypeSP type, TokenSP name, ExprSP value = nullptr) {
            this->type = type;
            this->name = name;
            this->value = value;
        }
        TypeSP type;
        TokenSP name;
        ExprSP value;
};

typedef std::shared_ptr<VarDeclStmt> VarDeclStmtSP;

class IfStmt : public Statement {
    public:
        IfStmt(ExprSP condition, StmtSP ifBody, StmtSP elseBody) {
            this->condition = condition;
            this->ifBody = ifBody;
            this->elseBody = elseBody;
        }
        ExprSP condition;
        StmtSP ifBody;
        StmtSP elseBody;
};

typedef std::shared_ptr<IfStmt> IfStmtSP;

class WhileStmt : public Statement {
    public:
        WhileStmt(ExprSP condition, StmtSP body, bool isForLoop = false) {
            this->condition = condition;
            this->body = body;
            this->isForLoop = isForLoop;
        }

        ExprSP condition;
        StmtSP body;
        bool isForLoop;
};
typedef std::shared_ptr<WhileStmt> WhileStmtSP;

class BreakStmt : public Statement {
    public:
        // BreakStmt();
};

typedef std::shared_ptr<BreakStmt> BreakStmtSP;

class ContinueStmt : public Statement {
    public:
        // ContinueStmt();
};

typedef std::shared_ptr<ContinueStmt> ContinueStmtSP;

#endif
