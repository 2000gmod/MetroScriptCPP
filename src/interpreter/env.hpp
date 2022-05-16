#ifndef ENV_HPP
#define ENV_HPP

#include <unordered_map>
#include <string>

#include "rtvar.hpp"
#include "../parser/stmt/stmt.hpp"


class VarEnv {
    public:
        VarEnv();
        VarEnv(VarEnv &enclosing);
        VarEnv *enclosing;

        RTimeVarSP &getVar(const std::string &name);
        FunctionDeclStmtSP &getFun(const std::string &name);

        void assignVar(const std::string &name, const RTimeVarSP &value);
        void defineVar(const std::string &name, const RTimeVarSP &value);

        void defineFun(const std::string &name, const FunctionDeclStmtSP &value);

        VarEnv &ancestor(int dist);
        
    private:
        std::unordered_map<std::string, RTimeVarSP> values;
        std::unordered_map<std::string, FunctionDeclStmtSP> funcs;
};

#endif