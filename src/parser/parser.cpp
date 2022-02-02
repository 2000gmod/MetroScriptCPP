#include "parser.hpp"

#include <memory>
#include <vector>

#include "../util/util.hpp"
#include "expr/expr.hpp"
#include "stmt/stmt.hpp"
#include "token/token.hpp"

Parser::Parser(std::vector<Token> tokens) { this->tokens = tokens; }

std::vector<StmtSP> Parser::parse() {
    std::vector<StmtSP> out;
    while (!isAtEnd()) {
        out.push_back(declaration());
    }
    return out;
}

StmtSP Parser::declaration() {
    if (match(TokenType::TYPE)) {
        const Token &typeToken = previous();
        if (match(TokenType::IDENTIFIER)) {
            const Token &name = previous();

            if (match(TokenType::ASSIGN)) {
                return varDeclaration(typeToken, name);
            }
            else if (match(TokenType::SEMICOLON)) {
                return std::make_shared<VarDeclStmt>(std::make_shared<Token>(typeToken), std::make_shared<Token>(name));
            }

            else if (match(TokenType::LEFT_PAREN)) {
                return functionDecl(typeToken, name);
            }
        }
    }
    return statement();
    // throw error(peek(), "Only declarations are allowed outside of a function scope.");
}

VarDeclStmtSP Parser::varDeclaration(Token type, Token name) {
    ExprSP initValue = expression();
    consume(TokenType::SEMICOLON, "Expected ';' after variable declaration.");

    return std::make_shared<VarDeclStmt>(std::make_shared<Token>(type), std::make_shared<Token>(name), initValue);
}

FunctionDeclStmtSP Parser::functionDecl(Token type, Token name) {
    std::vector<parameterT> params;

    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            if (params.size() >= 8) {
                error(peek(), "Cannot have more than 8 parameters.");
            }
            parameterT param = {std::make_shared<Token>(consume(TokenType::TYPE, "Expected a type.")),
                                std::make_shared<Token>(consume(TokenType::IDENTIFIER, "Expected an identifier."))};
            params.push_back(param);

        } while (match(TokenType::COMMA));
    }
    consume(TokenType::RIGHT_PAREN, "Expected ')' after parameter declaration.");
    consume(TokenType::LEFT_CUR, "Expected '{' after function declaration.");
    StmtSP funBlock = blockStatement();

    return std::make_shared<FunctionDeclStmt>(std::make_shared<Token>(type), std::make_shared<Token>(name), params,
                                              funBlock);
}

StmtSP Parser::statement() {
    if (match(TokenType::FOR)) return forStatement();
    if (match(TokenType::IF)) return ifStatement();
    if (match(TokenType::RETURN)) return returnStatement();
    if (match(TokenType::WHILE)) return whileStatement();
    if (match(TokenType::LEFT_CUR)) return blockStatement();

    return expressionStatement();
}

StmtSP Parser::expressionStatement() {
    ExprSP expr = expression();
    consume(TokenType::SEMICOLON, "Expected ';' after an expression.");
    return std::make_shared<ExprStmt>(expr);
}

StmtSP Parser::forStatement() {
    consume(TokenType::LEFT_PAREN, "Expected '(' after 'for'.");
    StmtSP init;

    if (match(TokenType::SEMICOLON)) {
        init = nullptr;
    }
    else if (match(TokenType::TYPE)) {
        const Token &type = previous();
        const Token &id = consume(TokenType::IDENTIFIER, "Expected identifier after variable declaration");
        consume(TokenType::ASSIGN, "Variable declaration without assignment is illegal in a for loop.");
        init = varDeclaration(type, id);
    }
    else {
        init = expressionStatement();
    }

    ExprSP condition = nullptr;

    if (!check(TokenType::SEMICOLON)) {
        condition = expression();
    }
    consume(TokenType::SEMICOLON, "Expected ';' after loop condition.");

    ExprSP increment = nullptr;

    if (!check(TokenType::RIGHT_PAREN)) {
        increment = expression();
    }
    consume(TokenType::RIGHT_PAREN, "Expected ')' after for increment expression.");

    StmtSP body = statement();
    if (increment != nullptr) {
        body = std::make_shared<BlockStmt>(std::vector<StmtSP> {body, std::make_shared<ExprStmt>(increment)});
    }
    if (condition == nullptr) condition = std::make_shared<ValueExpr>(std::make_shared<Token>(Token("true")));

    body = std::make_shared<WhileStmt>(condition, body);

    if (init != nullptr) {
        body = std::make_shared<BlockStmt>(std::vector<StmtSP> {init, body});
    }
    return body;
}

StmtSP Parser::ifStatement() {
    consume(TokenType::LEFT_PAREN, "Expected '(' after 'if'.");
    ExprSP condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expected ')' after if condition.");

    StmtSP ifBody = statement();
    StmtSP elseBody = nullptr;
    if (match(TokenType::ELSE)) {
        elseBody = statement();
    }

    return std::make_shared<IfStmt>(condition, ifBody, elseBody);
}

StmtSP Parser::returnStatement() {
    ExprSP value = nullptr;
    if (!check(TokenType::SEMICOLON)) {
        value = expression();
    }
    consume(TokenType::SEMICOLON, "Expected ';' after return statement.");
    return std::make_shared<ReturnStmt>(value);
}

StmtSP Parser::blockStatement() {
    std::vector<StmtSP> statements;
    while (!check(TokenType::RIGHT_CUR) && !isAtEnd()) {
        statements.push_back(declaration());
    }
    consume(TokenType::RIGHT_CUR, "Expected '}' after block statement.");
    return std::make_shared<BlockStmt>(statements);
}

StmtSP Parser::whileStatement() {
    consume(TokenType::LEFT_PAREN, "Expected '(' after while.");
    ExprSP condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expected ')' after while loop condition.");
    StmtSP body = statement();

    return std::make_shared<WhileStmt>(condition, body);
}

ExprSP Parser::expression() { return assignment(); }

ExprSP Parser::assignment() {
    ExprSP expr = orExpr();

    if (match(TokenType::ASSIGN)) {
        TokenSP equals = std::make_shared<Token>(previous());
        ExprSP value = assignment();

        if (instanceOf<IdentExpr>(expr.get())) {
            TokenSP name = ((IdentExpr *) expr.get())->name;
            return std::make_shared<AsignExpr>(name, value);
        }
        error(*equals.get(), "Invalid assignment target.");
    }
    return expr;
}

ExprSP Parser::orExpr() {
    ExprSP expr = andExpr();

    while (match(TokenType::OR)) {
        TokenSP op = std::make_shared<Token>(previous());
        ExprSP right = andExpr();
        expr = std::make_shared<BinaryExpr>(expr, right, op);
    }
    return expr;
}

ExprSP Parser::andExpr() {
    ExprSP expr = eqExpr();

    while (match(TokenType::AND)) {
        TokenSP op = std::make_shared<Token>(previous());
        ExprSP right = eqExpr();
        expr = std::make_shared<BinaryExpr>(expr, right, op);
    }

    return expr;
}

ExprSP Parser::eqExpr() {
    ExprSP expr = compExpr();

    while (match({TokenType::EQ, TokenType::NOT_EQ})) {
        TokenSP op = std::make_shared<Token>(previous());
        ExprSP right = compExpr();
        expr = std::make_shared<BinaryExpr>(expr, right, op);
    }

    return expr;
}

ExprSP Parser::compExpr() {
    ExprSP expr = addition();

    while (match({TokenType::GT, TokenType::GEQ, TokenType::LT, TokenType::LEQ})) {
        TokenSP op = std::make_shared<Token>(previous());
        ExprSP right = addition();
        expr = std::make_shared<BinaryExpr>(expr, right, op);
    }

    return expr;
}

ExprSP Parser::addition() {
    ExprSP expr = multiplication();

    while (match({TokenType::PLUS, TokenType::MINUS})) {
        TokenSP op = std::make_shared<Token>(previous());
        ExprSP right = multiplication();
        expr = std::make_shared<BinaryExpr>(expr, right, op);
    }

    return expr;
}

ExprSP Parser::multiplication() {
    ExprSP expr = unary();

    while (match({TokenType::STAR, TokenType::SLASH})) {
        TokenSP op = std::make_shared<Token>(previous());
        ExprSP right = unary();
        expr = std::make_shared<BinaryExpr>(expr, right, op);
    }

    return expr;
}

ExprSP Parser::unary() {
    if (match({TokenType::NOT, TokenType::MINUS})) {
        TokenSP op = std::make_shared<Token>(previous());
        ExprSP right = unary();
        return std::make_shared<UnaryExpr>(right, op);
    }
    return callExpr();
}

ExprSP Parser::callExpr() {
    ExprSP expr = primaryExpr();

    while (true) {
        if (match(TokenType::LEFT_PAREN)) {
            expr = finishCallExpr(expr);
        }
        else
            break;
    }
    return expr;
}

ExprSP Parser::finishCallExpr(ExprSP callee) {
    std::vector<ExprSP> args;
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            if (args.size() >= 8) {
                error(peek(), "Cannot have more than 8 arguments.");
            }
            args.push_back(expression());
        } while (match(TokenType::COMMA));
    }
    consume(TokenType::RIGHT_PAREN, "Expected ')' after argument list of call expression.");
    return std::make_shared<CallExpr>(args, callee);
}

ExprSP Parser::primaryExpr() {
    if (match({TokenType::INT_LIT, TokenType::DOUBLE_LIT, TokenType::STRING_LIT, TokenType::BOOL_LIT})) {
        return std::make_shared<ValueExpr>(std::make_shared<Token>(previous()));
    }
    if (match(TokenType::IDENTIFIER)) {
        return std::make_shared<IdentExpr>(std::make_shared<Token>(previous()));
    }

    if (match(TokenType::LEFT_PAREN)) {
        ExprSP expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expected ')' after expression.");
        return std::make_shared<GroupExpr>(expr);
    }

    throw error(peek(), "Expected expression.");
}

bool Parser::match(std::vector<TokenType> types) {
    for (TokenType &type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

const Token &Parser::consume(TokenType type, const char *message) {
    if (check(type)) return advance();
    throw error(peek(), message);
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().getType() == type;
}

const Token &Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::isAtEnd() { return peek().getType() == TokenType::EOFILE; }

const Token &Parser::peek() { return tokens[current]; }

const Token &Parser::previous() { return tokens[current - 1]; }

ParseException Parser::error(const Token &token, const char *message) {
    std::string messageString = "(at token \"" + toString(token) + "\") " + message;
    reportError(messageString);
    return ParseException(messageString);
}