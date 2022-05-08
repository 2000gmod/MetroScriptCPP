#ifndef RTVAR_HPP
#define RTVAR_HPP

#include "../parser/type/type.hpp"
#include "../parser/var/var.hpp"

struct RuntimeVariable {
    ~RuntimeVariable() = default;
};

typedef std::shared_ptr<RuntimeVariable> RTimeVarSP;


struct PrimitiveVar : public RuntimeVariable {
    Variable value;
    BasicTypeSP type;

    PrimitiveVar(const Variable &value, const BasicTypeSP &type) {
        this->value = value;
        this->type = type;
    }
};

typedef std::shared_ptr<PrimitiveVar> PrimitiveVarSP;

#endif