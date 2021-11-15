#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>
#include <memory>
#include <string>

template <class BaseType, class ComparedType>
inline bool instanceOf (const ComparedType *ptr) {
    return dynamic_cast<const BaseType *> (ptr) != nullptr;
}

inline void reportError (std::string message) { std::cout << "[[ Error reported: " << message << " ]]\n"; }

inline void reportError (std::string message, int line) {
    std::cout << "[[ Error reported at line " << line << ": " << message << " ]]\n";
}

#endif