#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "../parser/parser.hpp"
#include "rtvar.hpp"
#include "env.hpp"

class Interpreter {
    public:
        Interpreter(const std::vector<StmtSP> &statements);
        void run();
        void load();

        VarEnv &globals();

    private:
        
};

#endif