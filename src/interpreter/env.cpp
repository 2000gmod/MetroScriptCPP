#include "env.hpp"
#include "interpreter.hpp"

VarEnv::VarEnv() { 
    enclosing = nullptr;
}

VarEnv::VarEnv(VarEnv &enclosing) {
    this->enclosing = &enclosing;
}

RTimeVarSP &VarEnv::getVar(const std::string &name) {
    if (values.count(name) == 0) {
        if (enclosing == nullptr) throw RuntimeException(std::string("Variable not found: ") + name);
        else enclosing->getVar(name);
    }
    else return values[name];
}

FunctionDeclStmtSP &VarEnv::getFun(const std::string &name) {
    if (funcs.count(name) == 0) {
        if (enclosing == nullptr) throw RuntimeException(std::string("Function not found: ") + name);
        else enclosing->getFun(name);
    }
    else return funcs[name];
}

void VarEnv::assignVar(const std::string &name, const RTimeVarSP &value) {
    values.insert_or_assign(name, value);
}

void VarEnv::defineVar(const std::string &name, const RTimeVarSP &value) {
    values.insert({name, value});
}

void VarEnv::defineFun(const std::string &name, const FunctionDeclStmtSP &value) {
    funcs.insert({name, value});
}

VarEnv &VarEnv::ancestor(int dist) {
    if (dist <= 0) return *this;
    return enclosing->ancestor(dist - 1);
}