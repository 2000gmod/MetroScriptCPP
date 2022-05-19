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
    BasicTypeSP type;

    PrimitiveVar(const Variable &value, const BasicTypeSP &type) {
        this->value = value;
        this->type = type;
    }

    PrimitiveVar(const ValueExprSP &valExpr) {
        value = *(valExpr->value);
        const char *typeString;
        switch(valExpr->value->getType()) {
            case TokenType::INT_LIT:
                typeString = "int";
                break;
            case TokenType::DOUBLE_LIT:
                typeString = "double";
                break;
            case TokenType::STRING_LIT:
                typeString = "string";
                break;
            case TokenType::BOOL_LIT:
                typeString = "bool";
                break;
            default:
                break;
        }
        type->name = std::make_shared<Token>(Token(typeString));
    }

    PrimitiveVar(const RTimeVarSP &rt) {
        auto *rtp = (PrimitiveVar*) rt.get();
        if (!instanceOf<PrimitiveVar>(rtp)) throw RuntimeException("Trying to initialize primitive value with non primitive.");
        this->type = rtp->type;
        this->value = rtp->value;
    }

    PrimitiveVar(bool val) {
        type = std::make_shared<BasicType>(std::make_shared<Token>("bool"));
        value.boolValue = val;
        value.activeMember = 'b';
    }

    PrimitiveVar(int val) {
        type = std::make_shared<BasicType>(std::make_shared<Token>("int"));
        value.intValue = val;
        value.activeMember = 'i';
    }

    PrimitiveVar(double val) {
        type = std::make_shared<BasicType>(std::make_shared<Token>("double"));
        value.doubleValue = val;
        value.activeMember = 'i';
    }

    PrimitiveVar(const std::string &val) {
        type = std::make_shared<BasicType>(std::make_shared<Token>("string"));
        value.stringValue = val;
        value.activeMember = 's';
    }


    const std::string getType() const {
        return type->name->getTypeName();
    }

    bool isNumeric() const {
        return (type->name->getTypeName() == "int" || type->name->getTypeName() == "double");
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