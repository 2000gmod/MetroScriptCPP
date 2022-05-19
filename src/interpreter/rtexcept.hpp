#ifndef RTEXCEPT_HPP
#define RTEXCEPT_HPP

#include <string>
#include <exception>

struct RuntimeException : public std::exception {
    std::string message;
    explicit RuntimeException(const std::string &msg) : message(msg) {}
    const char *what() {
        return message.c_str();
    }
};

#endif