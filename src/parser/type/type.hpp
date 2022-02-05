#ifndef TYPE_HPP
#define TYPE_HPP

#include <memory>
#include "../token/token.hpp"


class Type {
    public:
        virtual ~Type() = default;
};

typedef std::shared_ptr<Type> TypeSP;

class BasicType : public Type {
    public:
        BasicType(TokenSP name) {
            this->name = name;
        }
        TokenSP name;
};

typedef std::shared_ptr<BasicType> BasicTypeSP;

class ArrayType : public Type {
    public:
        ArrayType(TypeSP elemType) {
            this->elemType = elemType;
        }
        TypeSP elemType;
};

typedef std::shared_ptr<ArrayType> ArrayTypeSP;

class FuncPtrType : public Type {
    public:
        FuncPtrType(TypeSP returnType) {
            this->returnType = returnType;
        }
        TypeSP returnType;
};

typedef std::shared_ptr<FuncPtrType> FuncPtrTypeSP;

#endif