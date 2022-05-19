#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "../parser/parser.hpp"
#include "rtvar.hpp"
#include "env.hpp"

#include "rtexcept.hpp"

struct ReturnValueContainer {
    RTimeVarSP value;
    ReturnValueContainer(const RTimeVarSP &val) : value(val) {}
};

struct LoopControlException {
    bool isContinue;
    LoopControlException(bool cont) : isContinue(cont) {}
};


class Interpreter {
    public:
        Interpreter(const std::vector<StmtSP> &statements);
        void run();
        void load();


    private:
        std::vector<StmtSP> statementList;
        VarEnv globalEnv, *innermost;

        void enterMainFunction(const FunctionDeclStmtSP &mainFun);
        void runFunction(const FunctionDeclStmtSP &fun);

        void runStatement(const StmtSP &s);
        void runBlockStmt(const StmtSP &s);
        void runExprStmt(const StmtSP &s);
        void runReturnStmt(const StmtSP &s);
        void runVarDeclStmt(const StmtSP &s);
        void runIfStmt(const StmtSP &s);
        void runWhileStmt(const StmtSP &s);
        void runBreakStmt();
        void runContinueStmt();

        RTimeVarSP evaluateExpr(const ExprSP &e);
        RTimeVarSP evalValueExpr(const ExprSP &e);
        RTimeVarSP evalIdentExpr(const ExprSP &e);
        RTimeVarSP evalUnaryExpr(const ExprSP &e);
        RTimeVarSP evalAsignExpr(const ExprSP &e);
        RTimeVarSP evalBinaryExpr(const ExprSP &e);
        RTimeVarSP evalGroupExpr(const ExprSP &e);
        RTimeVarSP evalCallExpr(const ExprSP &e);
        RTimeVarSP evalCastingExpr(const ExprSP &e);
        RTimeVarSP evalSubscriptExpr(const ExprSP &e);
        
        bool isTrue(const ExprSP &e);
};

#endif