#include "interpreter.hpp"

#include "../util/util.hpp"
#include "../util/debug.hpp"

Interpreter::Interpreter(const std::vector<StmtSP> &statements) {
    statementList = statements;
}

void Interpreter::run() {
    for (const auto &f : statementList) {
        auto *fPtr = (FunctionDeclStmt*) f.get();
        if (fPtr->name->getName() == "main") {
            enterMainFunction(std::dynamic_pointer_cast<FunctionDeclStmt>(f));
            return;
        }
    }
    reportError("No main function found.");
    exit(EXIT_FAILURE);
}

void Interpreter::load() {
    for (const auto &f : statementList) {
        Statement *ptr = f.get();
        if (!instanceOf<FunctionDeclStmt>(ptr)) {
            reportError("Tried to load a top-level statement which was not a function.");
            exit(EXIT_FAILURE);
        }
        auto *fPtr = (FunctionDeclStmt*) ptr;

        globalEnv.defineFun(fPtr->name->getName(), std::dynamic_pointer_cast<FunctionDeclStmt>(f));
        //Debug::log(std::string("Loaded function: ") + fPtr->name->getName());
    }
}

void Interpreter::enterMainFunction(const FunctionDeclStmtSP &mainFun) {
    VarEnv mainScope(globalEnv);
    innermost = &mainScope;
    
    // Handle main args here

    runFunction(mainFun);
}

void Interpreter::runFunction(const FunctionDeclStmtSP &fun) {
    auto *block = (BlockStmt*) fun->body.get();

    for (const auto &s : block->statements) {
        std::cout << s << "\n";
    }
}