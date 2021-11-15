#ifndef MATHPARSER_HPP
#define MATHPARSER_HPP

#include <vector>

#include "../expr/expr.hpp"
#include "../token/token.hpp"
#include "../var/var.hpp"

std::shared_ptr<Expression> parseMath (Token *tokenPtr);
#endif