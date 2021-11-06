#ifndef MATHPARSER_HPP
#define MATHPARSER_HPP

#include <vector>

#include "../token/token.hpp"
#include "../var/var.hpp"
#include "../expr/expr.hpp"

std::shared_ptr<Expression> parseMath(Token* tokenPtr);
#endif