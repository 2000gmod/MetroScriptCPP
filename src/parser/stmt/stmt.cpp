#include "stmt.hpp"
#include "../../interpreter/interpreter.hpp"

BlockStmt::BlockStmt(const std::vector<StmtSP>& statements) { 
    this->statements = statements;
    runFunction = &Interpreter::runBlockStmt;
}

ExprStmt::ExprStmt(const ExprSP &expr) { 
    this->expr = expr;
    runFunction = &Interpreter::runExprStmt;
}

FunctionDeclStmt::FunctionDeclStmt(const TypeSP &type, const TokenSP &name, const std::vector<parameterT> &parameters, const StmtSP &body) {
    this->type = type;
    this->name = name;
    this->parameters = parameters;
    this->body = body;
    runFunction = nullptr;
}

ReturnStmt::ReturnStmt(const ExprSP &value) { 
    this->value = value; 
    runFunction = &Interpreter::runReturnStmt;
}

VarDeclStmt::VarDeclStmt(const TypeSP &type, const TokenSP &name, const ExprSP &value) {
    this->type = type;
    this->name = name;
    this->value = value;
    runFunction = &Interpreter::runVarDeclStmt;
}

IfStmt::IfStmt(const ExprSP &condition, const StmtSP &ifBody, const StmtSP &elseBody) {
    this->condition = condition;
    this->ifBody = ifBody;
    this->elseBody = elseBody;
    runFunction = &Interpreter::runIfStmt;
}

WhileStmt::WhileStmt(const ExprSP &condition, const StmtSP &body, bool isForLoop) {
    this->condition = condition;
    this->body = body;
    this->isForLoop = isForLoop;
    runFunction = &Interpreter::runWhileStmt;
}

BreakStmt::BreakStmt() {
    runFunction = &Interpreter::runBreakStmt;
}

ContinueStmt::ContinueStmt() {
    runFunction = &Interpreter::runContinueStmt;
}