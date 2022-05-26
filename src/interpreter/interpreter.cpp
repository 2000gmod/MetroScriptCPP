#include "interpreter.hpp"

#include "../util/util.hpp"
#include "../util/debug.hpp"

Interpreter::Interpreter(const std::vector<StmtSP> &statements) {
    statementList = statements;
    innermost = &globalEnv;
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

    try {
        runFunction(mainFun);
    }
    catch (ReturnValueContainer) {
        //std::cout << "Finished!\n";
    }
}

void Interpreter::runFunction(const FunctionDeclStmtSP &fun) {
    runStatement(fun->body);
}

void Interpreter::runStatement(const StmtSP &s) {
    auto *sptr = s.get();

    if (instanceOf<BlockStmt>(sptr)) {
        runBlockStmt(s);
    }
    else if (instanceOf<ExprStmt>(sptr)) {
        runExprStmt(s);
    }
    else if (instanceOf<ReturnStmt>(sptr)) {
        runReturnStmt(s);
    }
    else if (instanceOf<VarDeclStmt>(sptr)) {
        runVarDeclStmt(s);
    }
    else if (instanceOf<IfStmt>(sptr)) {
        runIfStmt(s);
    }
    else if (instanceOf<WhileStmt>(sptr)) {
        runWhileStmt(s);
    }
    else if (instanceOf<BreakStmt>(sptr)) {
        runBreakStmt();
    }
    else if (instanceOf<ContinueStmt>(sptr)) {
        runContinueStmt();
    } 
    else {
        reportError("Invalid statement.");
        exit(EXIT_FAILURE);
    }
}

void Interpreter::runBlockStmt(const StmtSP &s) {
    VarEnv newScope(*innermost);
    innermost = &newScope;
    
    auto *block = (BlockStmt*) s.get();

    for (const auto &s : block->statements) {
        runStatement(s);
    }

    innermost = newScope.enclosing;
}

void Interpreter::runExprStmt(const StmtSP &s) {
    auto *expr = (ExprStmt*) s.get();
    evaluateExpr(expr->expr);
}

void Interpreter::runReturnStmt(const StmtSP &s) {
    auto *rstmt = (ReturnStmt*) s.get();
    throw ReturnValueContainer(
            rstmt->value == nullptr ? nullptr : evaluateExpr(rstmt->value)
        );
}

void Interpreter::runVarDeclStmt(const StmtSP &s) {
    auto *decl = (VarDeclStmt*) s.get();
    auto expr = std::make_shared<PrimitiveVar>(evaluateExpr(decl->value));
    innermost->defineVar(decl->name->getName(), expr);
}

void Interpreter::runIfStmt(const StmtSP &s) {
    auto *ifst = (IfStmt*) s.get();
    if (isTrue(ifst->condition)) {
        runStatement(ifst->ifBody);
    }
    else if (ifst->elseBody != nullptr) runStatement(ifst->elseBody);
}

void Interpreter::runWhileStmt(const StmtSP &s) {
    auto *whst = (WhileStmt*) s.get();

    while (isTrue(whst->condition)) {
        try {
            runStatement(whst->body);
        }
        catch (LoopControlException e) {
            if (!e.isContinue) {
                return;
            }
            else {
                if (!whst->isForLoop) continue;
                else {
                    auto *block = (BlockStmt*) whst->body.get();
                    runStatement(block->statements.back());
                    continue;
                }
            }
        }
    }
}

void Interpreter::runBreakStmt() {
    throw LoopControlException(false);
}

void Interpreter::runContinueStmt() {
    throw LoopControlException(true);
}

RTimeVarSP Interpreter::evaluateExpr(const ExprSP &e) {
    auto *ptr = e.get();
    
    if (instanceOf<ValueExpr>(ptr)) {
        return evalValueExpr(e);
    }
    else if (instanceOf<IdentExpr>(ptr)) {
        return evalIdentExpr(e);
    }
    else if (instanceOf<UnaryExpr>(ptr)) {
        return evalUnaryExpr(e);
    }
    else if (instanceOf<AsignExpr>(ptr)) {
        return evalAsignExpr(e);
    }
    else if (instanceOf<BinaryExpr>(ptr)) {
        return evalBinaryExpr(e);
    }
    else if (instanceOf<GroupExpr>(ptr)) {
        return evalGroupExpr(e);
    }
    else if (instanceOf<CallExpr>(ptr)) {
        return evalCallExpr(e);
    }
    else if (instanceOf<CastingExpr>(ptr)) {
        return evalCastingExpr(e);
    }
    else if (instanceOf<SubscriptExpr>(ptr)) {
        return evalSubscriptExpr(e);
    }
    else {
        reportError("Invalid expression.");
        exit(EXIT_FAILURE);
    }
}

RTimeVarSP Interpreter::evalValueExpr(const ExprSP &e) {
    auto *ptr = (ValueExpr*) e.get();
    auto tok = ptr->value;
    auto type = tok->getType();
    BasicTypeSP typeExpr;

    switch(type) {
        case TokenType::INT_LIT:
            typeExpr = std::make_shared<BasicType>(std::make_shared<Token>("int"));
            break;
        case TokenType::DOUBLE_LIT:
            typeExpr = std::make_shared<BasicType>(std::make_shared<Token>("double"));
            break;
        case TokenType::STRING_LIT:
            typeExpr = std::make_shared<BasicType>(std::make_shared<Token>("string"));
            break;
        case TokenType::BOOL_LIT:
            typeExpr = std::make_shared<BasicType>(std::make_shared<Token>("bool"));
            break;
        default:
            throw RuntimeException("Invalid value expression.");
    }
    
   return std::make_shared<PrimitiveVar>(*evaluate(e), typeExpr);
}

RTimeVarSP Interpreter::evalIdentExpr(const ExprSP &e) {
    auto *ptr = (IdentExpr*) e.get();
    return (innermost->getVar(ptr->name->getName()));
}

RTimeVarSP Interpreter::evalUnaryExpr(const ExprSP &e) {
    auto ep = std::dynamic_pointer_cast<UnaryExpr>(e);
    PrimitiveVar out(evaluateExpr(ep->expr));
    switch(ep->op->getType()) {
        case TokenType::PLUS:
            out = +out;
            break;
        case TokenType::MINUS:
            out = -out;
            break;
        case TokenType::NOT:
            out = !out;
            break;
        default:
            break;
    }
    return std::make_shared<PrimitiveVar>(out);
}

RTimeVarSP Interpreter::evalAsignExpr(const ExprSP &e) {
    auto ep = std::dynamic_pointer_cast<AsignExpr>(e);
    auto out = std::make_shared<PrimitiveVar>(evaluateExpr(ep->expr));

    innermost->assignVar(ep->name->getName(), out);
    return out;
}

RTimeVarSP Interpreter::evalBinaryExpr(const ExprSP &e) {
    auto ep = std::dynamic_pointer_cast<BinaryExpr>(e);
    auto left = std::make_shared<PrimitiveVar>(evaluateExpr(ep->l));
    auto right = std::make_shared<PrimitiveVar>(evaluateExpr(ep->r));
    auto &op = ep->op;
    

    switch (op->getType()) {
        case TokenType::PLUS:
            return std::make_shared<PrimitiveVar>(*left + *right);
        case TokenType::MINUS:
            return std::make_shared<PrimitiveVar>(*left - *right);
        case TokenType::STAR:
            return std::make_shared<PrimitiveVar>(*left * *right);
        case TokenType::SLASH:
            return std::make_shared<PrimitiveVar>(*left / *right);
        case TokenType::MOD:
            return std::make_shared<PrimitiveVar>(*left % *right);
        case TokenType::EQ:
            return std::make_shared<PrimitiveVar>(*left == *right);
        case TokenType::NOT_EQ:
            return std::make_shared<PrimitiveVar>(*left != *right);
        case TokenType::GT:
            return std::make_shared<PrimitiveVar>(*left > *right);
        case TokenType::GEQ:
            return std::make_shared<PrimitiveVar>(*left >= *right);
        case TokenType::LT:
            return std::make_shared<PrimitiveVar>(*left < *right);
        case TokenType::LEQ:
            return std::make_shared<PrimitiveVar>(*left <= *right);
        case TokenType::AND:
            return std::make_shared<PrimitiveVar>(*left && *right);
        case TokenType::OR:
            return std::make_shared<PrimitiveVar>(*left || *right);
        default:
            throw RuntimeException("Invalid binary expr.");
    }
}

RTimeVarSP Interpreter::evalGroupExpr(const ExprSP &e) {
    auto *ptr = (GroupExpr*) e.get();
    return(evaluateExpr(ptr->expr));
}

RTimeVarSP Interpreter::evalCallExpr(const ExprSP &e) {
    auto *ptr = (CallExpr*) e.get();

    if (!instanceOf<IdentExpr>(ptr->callee.get())) {
        throw RuntimeException("Only identifiers are valid function names.");
    }

    auto *idptr = (IdentExpr*) ptr->callee.get();
    const std::string &name = idptr->name->getName();

    try {
        interceptBuiltIns(name, ptr->args);
    }
    catch (ReturnValueContainer e) {
        return e.value;
    }

    const auto &fun = innermost->getFun(name);

    if (ptr->args.size() != fun->parameters.size()) {
        throw RuntimeException("Invalid number of arguments.");
    }
    //return std::make_shared<PrimitiveVar>("Placeholder");

    VarEnv newScope(*innermost);
    innermost = &newScope;

    RTimeVarSP out;
    try {
        int i = 0;
        for (auto &arg : ptr->args) {
            newScope.defineVar(std::get<1>(fun->parameters[i])->getName(), evaluateExpr(arg));
            i++;
        }
        runFunction(fun);
    }
    catch (ReturnValueContainer &e) {
        out = e.value;
    }
    innermost = newScope.enclosing;
    return out;
}

RTimeVarSP Interpreter::evalCastingExpr(const ExprSP &e) {
    auto *ptr = (CastingExpr*) e.get();
    return evaluateExpr(ptr->expr);
}

RTimeVarSP Interpreter::evalSubscriptExpr(const ExprSP &e) {
    auto *ptr = (SubscriptExpr*) e.get();
    return evaluateExpr(ptr->index);
}

bool Interpreter::isTrue(const ExprSP &e) {
    PrimitiveVar expr(evaluateExpr(e));
    return expr && true;
}

void Interpreter::interceptBuiltIns(const std::string &name, const std::vector<ExprSP> &args) {
    if (name == "print") internalPrint(args);
    return;
}

void Interpreter::internalPrint(const std::vector<ExprSP> &args) {
    if (args.size() == 0) return;
    //std::cout << args.size() << "\n";

    for (const auto &e : args) {
        std::cout << evaluateExpr(e)->toString();
    }
    throw ReturnValueContainer(nullptr);
}