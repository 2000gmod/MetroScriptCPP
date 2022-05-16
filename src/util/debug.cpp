#include "debug.hpp"

#include <iostream>

void Debug::log(const std::string &msg) {
    std::cout << "[Debug::Log] ::> " << msg << std::endl;
}