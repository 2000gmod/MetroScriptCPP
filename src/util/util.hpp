#ifndef UTIL_HPP
#define UTIL_HPP

#include <memory>

template <class BaseType, class ComparedType>
inline bool instanceOf(const ComparedType* ptr) {
    return dynamic_cast<const BaseType*>(ptr) != nullptr;
}

#endif