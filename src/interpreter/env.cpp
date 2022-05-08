#include "env.hpp"

VarEnv::VarEnv() { 
    enclosing = nullptr;
}

VarEnv::VarEnv(VarEnv &enclosing) {
    this->enclosing = &enclosing;
}