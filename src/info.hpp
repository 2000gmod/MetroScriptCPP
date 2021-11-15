#ifndef INFO_HPP
#define INFO_HPP

#include <iostream>

#define MSCPP_VERSION "preview-0.1"

inline void printInfo () {
    std::cout << "|| \tMetroScriptCPPI -- V" << MSCPP_VERSION << " \t||\n";
    std::cout << "|| \tGCC " << __VERSION__ << " -- " << __DATE__ << ", " << __TIME__ << "\t||\n";
}

#endif