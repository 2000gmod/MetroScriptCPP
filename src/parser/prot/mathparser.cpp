#include "mathparser.hpp"

#include <iostream>

int tokenArraySize(Token *tokens) {
    int i = 0;
    while (tokens[i].getType() != TokenType::EOFILE) {
        i++;
    }
    return i;
}

int findMatchingParenthesisIndex(Token *tokens, int index) {
    auto len = tokenArraySize(tokens);
    int parenthesisLevel = 1;

    for (auto i = index + 1; i < len; i++) {
        auto t = tokens[i];
        if (t.getType() == TokenType::LEFT_PAREN)
            parenthesisLevel++;
        else if (t.getType() == TokenType::RIGHT_PAREN)
            parenthesisLevel--;

        if (parenthesisLevel == 0) return i;
    }
    return 0;
}

ExprSP parseMath(Token *tokensPtr) {
    auto size = tokenArraySize(tokensPtr);
    int parenthesisLevel = 0;

    if (size == 1 && tokensPtr[0].isLiteral()) {
        Token *copy = new Token(tokensPtr[0]);
        return ValueExprSP(new ValueExpr(TokenSP(copy)));
    }

    if (tokensPtr[0].getType() == TokenType::LEFT_PAREN && tokensPtr[size - 1].getType() == TokenType::RIGHT_PAREN
        && findMatchingParenthesisIndex(tokensPtr, 0) == size - 1)
    {
        tokensPtr[size - 1].nullify();
        return GroupExprSP(new GroupExpr(ExprSP(parseMath(&tokensPtr[1]))));
    }

    for (auto i = size - 1; i >= 0; i--) {
        Token &tk = tokensPtr[i];
        TokenType type = tk.getType();
        if (type == TokenType::LEFT_PAREN) parenthesisLevel--;
        if (type == TokenType::RIGHT_PAREN) parenthesisLevel++;

        if (parenthesisLevel == 0 && (type == TokenType::PLUS || type == TokenType::MINUS)) {
            tokensPtr[i].nullify();
            Token *opPtr = type == TokenType::PLUS ? new Token("+") : new Token("-");

            TokenSP op(opPtr);
            ExprSP left(parseMath(tokensPtr));
            ExprSP right(parseMath(&tokensPtr[i + 1]));

            if (i == 0 || tokensPtr[i - 1].isOperator()) return UnaryExprSP(new UnaryExpr(right, op));
            return BinaryExprSP(new BinaryExpr(left, right, op));
        }
    }

    for (auto i = size - 1; i >= 0; i--) {
        Token &tk = tokensPtr[i];
        TokenType type = tk.getType();
        if (type == TokenType::LEFT_PAREN) parenthesisLevel--;
        if (type == TokenType::RIGHT_PAREN) parenthesisLevel++;

        if (parenthesisLevel == 0 && (type == TokenType::STAR || type == TokenType::SLASH)) {
            tokensPtr[i].nullify();
            Token *opPtr = (type == TokenType::STAR ? new Token("*") : new Token("/"));

            TokenSP op(opPtr);
            ExprSP left(parseMath(tokensPtr));
            ExprSP right(parseMath(&tokensPtr[i + 1]));
            return BinaryExprSP(new BinaryExpr(left, right, op));
        }
    }

    return nullptr;
}