#include "rtvar.hpp"

PrimitiveVar &operator +(PrimitiveVar &rhs) {
    if (!rhs.isNumeric()) throw RuntimeException("Cannot use unary + operator on non-numerics");
    return rhs;
}

PrimitiveVar &operator -(PrimitiveVar &rhs) {
    if (!rhs.isNumeric()) throw RuntimeException("Cannot use unary - operator on non-numerics");
    //PrimitiveVar a(rhs.value, rhs.type);
    if (rhs.value.activeMember == 'd') {
        rhs.value.doubleValue *= -1;
    }
    else rhs.value.intValue *= -1;
    return rhs;
}

PrimitiveVar &operator !(PrimitiveVar &rhs) {
    if (rhs.value != 'b') throw RuntimeException("Cannot use unary ! on non-bool values");
    rhs.value.boolValue = !rhs.value.boolValue;
    return rhs;
}


PrimitiveVar operator +(const PrimitiveVar &lhs, const PrimitiveVar &rhs) {
    return PrimitiveVar(lhs.value + rhs.value);
}

PrimitiveVar operator -(const PrimitiveVar &lhs, const PrimitiveVar &rhs) {
    return PrimitiveVar(lhs.value - rhs.value);
}

PrimitiveVar operator *(const PrimitiveVar &lhs, const PrimitiveVar &rhs) {
    return PrimitiveVar(lhs.value * rhs.value);
}

PrimitiveVar operator /(const PrimitiveVar &lhs, const PrimitiveVar &rhs) {
    return PrimitiveVar(lhs.value / rhs.value);
}

PrimitiveVar operator %(const PrimitiveVar &lhs, const PrimitiveVar &rhs) {
    return PrimitiveVar(lhs.value % rhs.value);
}


bool operator ==(const PrimitiveVar &lhs, const PrimitiveVar &rhs) {
    return lhs.value == rhs.value;
}

bool operator !=(const PrimitiveVar &lhs, const PrimitiveVar &rhs) {
    return lhs.value != rhs.value;
}

bool operator <(const PrimitiveVar &lhs, const PrimitiveVar &rhs) {
    return lhs.value < rhs.value;
}

bool operator >(const PrimitiveVar &lhs, const PrimitiveVar &rhs) {
    return lhs.value > rhs.value;
}

bool operator >=(const PrimitiveVar &lhs, const PrimitiveVar &rhs) {
    return lhs.value >= rhs.value;
}

bool operator <=(const PrimitiveVar &lhs, const PrimitiveVar &rhs) {
    return lhs.value <= rhs.value;
}

bool operator &&(const PrimitiveVar &lhs, const PrimitiveVar &rhs) {
    return lhs.value && rhs.value;
}

bool operator ||(const PrimitiveVar &lhs, const PrimitiveVar &rhs) {
    return lhs.value || rhs.value;
}

std::string PrimitiveVar::toString() const {
    return value.operator std::string();
}