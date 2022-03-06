#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "../parser/parser.hpp"

class Interpreter {
    public:
        Interpreter(const std::vector<StmtSP> &statements);
        void run();
        void load();

};

#endif