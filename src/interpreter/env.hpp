#ifndef ENV_HPP
#define ENV_HPP

#include <unordered_map>
#include <string>

#include "rtvar.hpp"

class VarEnv {
    public:
        VarEnv();
        VarEnv(VarEnv &enclosing);
        VarEnv *enclosing;

        RuntimeVariable &get(const std::string &name);
        void assign(const std::string &name, const RTimeVarSP &value);
        void define(const std::string &name, const RTimeVarSP &value);

        VarEnv &ancestor(int dist);
        
    private:
        std::unordered_map<std::string, RTimeVarSP> values;
};

#endif