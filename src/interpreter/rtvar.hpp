#ifndef RTVAR_HPP
#define RTVAR_HPP

#include "../parser/type/type.hpp"
#include "../parser/var/var.hpp"
#include "../parser/expr/expr.hpp"
#include "../util/util.hpp"
#include "rtexcept.hpp"

struct RuntimeVariable {
    ~RuntimeVariable() = default;
    virtual std::string toString() const = 0;
};

typedef std::shared_ptr<RuntimeVariable> RTimeVarSP;


struct PrimitiveVar : public RuntimeVariable {
    Variable value;

    PrimitiveVar(const Variable &value) {
        this->value = value;
    }

    PrimitiveVar(const ValueExprSP &valExpr) {
        value = *(valExpr->value);
    }

    PrimitiveVar(const RTimeVarSP &rt) {
        auto *rtp = (PrimitiveVar*) rt.get();
        if (!instanceOf<PrimitiveVar>(rtp)) throw RuntimeException("Trying to initialize primitive value with non primitive.");
        this->value = rtp->value;
    }

    PrimitiveVar(bool val) {
        value.boolValue = val;
        value.activeMember = 'b';
    }

    PrimitiveVar(int val) {
        value.intValue = val;
        value.activeMember = 'i';
    }

    PrimitiveVar(double val) {
        value.doubleValue = val;
        value.activeMember = 'i';
    }

    PrimitiveVar(const std::string &val) {
        value.stringValue = val;
        value.activeMember = 's';
    }


    bool isNumeric() const {
        return (value.activeMember == 'd' || value.activeMember == 'i');
    }

    virtual std::string toString() const override;
};

PrimitiveVar &operator +(PrimitiveVar &rhs);
PrimitiveVar &operator -(PrimitiveVar &rhs);
PrimitiveVar &operator !(PrimitiveVar &rhs);

PrimitiveVar operator +(const PrimitiveVar &lhs, const PrimitiveVar &rhs);
PrimitiveVar operator -(const PrimitiveVar &lhs, const PrimitiveVar &rhs);
PrimitiveVar operator *(const PrimitiveVar &lhs, const PrimitiveVar &rhs);
PrimitiveVar operator /(const PrimitiveVar &lhs, const PrimitiveVar &rhs);
PrimitiveVar operator %(const PrimitiveVar &lhs, const PrimitiveVar &rhs);

bool operator ==(const PrimitiveVar &lhs, const PrimitiveVar &rhs);
bool operator !=(const PrimitiveVar &lhs, const PrimitiveVar &rhs);
bool operator <(const PrimitiveVar &lhs, const PrimitiveVar &rhs);
bool operator >(const PrimitiveVar &lhs, const PrimitiveVar &rhs);
bool operator >=(const PrimitiveVar &lhs, const PrimitiveVar &rhs);
bool operator <=(const PrimitiveVar &lhs, const PrimitiveVar &rhs);

bool operator &&(const PrimitiveVar &lhs, const PrimitiveVar &rhs);
bool operator ||(const PrimitiveVar &lhs, const PrimitiveVar &rhs);

typedef std::shared_ptr<PrimitiveVar> PrimitiveVarSP;

#endif