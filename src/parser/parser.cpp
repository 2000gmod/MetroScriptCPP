#include "parser.hpp"

#include "../util/util.hpp"
#include "stmt/stmt.hpp"
#include "token/token.hpp"

Parser::Parser(std::vector<Token> tokens) { this->tokens = tokens; }

bool Parser::match(std::vector<TokenType> types) {
    for (TokenType &type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

Token Parser::consume(TokenType type, std::string &message) {
    if (check(type)) return advance();
    throw error(peek(), message);
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().getType() == type;
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::isAtEnd() { return peek().getType() == TokenType::EOFILE; }

Token Parser::peek() { return tokens[current]; }

Token Parser::previous() { return tokens[current - 1]; }

ParseException Parser::error(const Token &token, std::string &message) {
    reportError(message);
    return ParseException(message);
}