#ifndef LINEPR_HPP
#define LINEPR_HPP

#include <string>
#include <vector>

#include "token.hpp"

#define MAX_SCANNER_RANGE 32

std::vector<Token> tokenizeString(std::string input);
std::vector<std::string> getFileLines(std::string fileContent);

#endif