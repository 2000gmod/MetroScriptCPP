#ifndef STMT_HPP
#define STMT_HPP

#include <memory>
#include <tuple>
#include <vector>

#include "../expr/expr.hpp"
#include "../token/token.hpp"

class Interpreter;
class Statement;

typedef std::shared_ptr<Statement> StmtSP;

class Statement {
    public:
        virtual ~Statement() = default;
        void (Interpreter::*runFunction)(const StmtSP&) = 0;
};


class BlockStmt : public Statement {
    public:
        BlockStmt(const std::vector<StmtSP>& statements);
        std::vector<StmtSP> statements;
};

typedef std::shared_ptr<BlockStmt> BlockStmtSP;

class ExprStmt : public Statement {
    public:
        ExprStmt(const ExprSP &expr);
        ExprSP expr;
};

typedef std::shared_ptr<ExprStmt> ExprStmtSP;

typedef std::tuple<TypeSP, TokenSP> parameterT;
class FunctionDeclStmt : public Statement {
    public:
        FunctionDeclStmt(const TypeSP &type, const TokenSP &name, const std::vector<parameterT> &parameters, const StmtSP &body);
        TypeSP type;
        TokenSP name;
        std::vector<parameterT> parameters;
        StmtSP body;
};

typedef std::shared_ptr<FunctionDeclStmt> FunctionDeclStmtSP;

class ReturnStmt : public Statement {
    public:
        ReturnStmt(const ExprSP &value);
        ExprSP value;
};

typedef std::shared_ptr<ReturnStmt> ReturnStmtSP;

class VarDeclStmt : public Statement {
    public:
        VarDeclStmt(const TypeSP &type, const TokenSP &name, const ExprSP &value = nullptr);
        TypeSP type;
        TokenSP name;
        ExprSP value;
};

typedef std::shared_ptr<VarDeclStmt> VarDeclStmtSP;

class IfStmt : public Statement {
    public:
        IfStmt(const ExprSP &condition, const StmtSP &ifBody, const StmtSP &elseBody);
        ExprSP condition;
        StmtSP ifBody;
        StmtSP elseBody;
};

typedef std::shared_ptr<IfStmt> IfStmtSP;

class WhileStmt : public Statement {
    public:
        WhileStmt(const ExprSP &condition, const StmtSP &body, bool isForLoop = false);
        ExprSP condition;
        StmtSP body;
        bool isForLoop;
};
typedef std::shared_ptr<WhileStmt> WhileStmtSP;

class BreakStmt : public Statement {
    public:
        BreakStmt();
};

typedef std::shared_ptr<BreakStmt> BreakStmtSP;

class ContinueStmt : public Statement {
    public:
        ContinueStmt();
};

typedef std::shared_ptr<ContinueStmt> ContinueStmtSP;

#endif
