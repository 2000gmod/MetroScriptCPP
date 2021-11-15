#include "token.hpp"

#include "../../util/util.hpp"

using std::string;

string toString(const TokenType &tp) {
    string out = "";

    switch (tp) {
        case TokenType::PLUS:
            out += "PLUS";
            break;
        case TokenType::MINUS:
            out += "MINUS";
            break;
        case TokenType::STAR:
            out += "STAR";
            break;
        case TokenType::SLASH:
            out += "SLASH";
            break;
        case TokenType::COMMA:
            out += "COMMA";
            break;
        case TokenType::DOT:
            out += "DOT";
            break;
        case TokenType::MOD:
            out += "MOD";
            break;
        case TokenType::ASSIGN:
            out += "ASSIGN";
            break;
        case TokenType::EQ:
            out += "EQ";
            break;
        case TokenType::NOT_EQ:
            out += "NOT_EQ";
            break;
        case TokenType::LT:
            out += "LT";
            break;
        case TokenType::GT:
            out += "GT";
            break;
        case TokenType::LEQ:
            out += "LEQ";
            break;
        case TokenType::GEQ:
            out += "GEQ";
            break;
        case TokenType::SEMICOLON:
            out += "SEMICOLON";
            break;
        case TokenType::AND:
            out += "AND";
            break;
        case TokenType::NOT:
            out += "NOT";
            break;
        case TokenType::OR:
            out += "OR";
            break;
        case TokenType::LEFT_PAREN:
            out += "LEFT_PAREN";
            break;
        case TokenType::RIGHT_PAREN:
            out += "RIGHT_PAREN";
            break;
        case TokenType::LEFT_SQR:
            out += "LEFT_SQR";
            break;
        case TokenType::RIGHT_SQR:
            out += "RIGHT_SQR";
            break;
        case TokenType::LEFT_CUR:
            out += "LEFT_CUR";
            break;
        case TokenType::RIGHT_CUR:
            out += "RIGHT_CUR";
            break;
        case TokenType::IF:
            out += "IF";
            break;
        case TokenType::ELSE:
            out += "ELSE";
            break;
        case TokenType::FOR:
            out += "FOR";
            break;
        case TokenType::WHILE:
            out += "WHILE";
            break;
        case TokenType::BREAK:
            out += "BREAK";
            break;
        case TokenType::CONTINUE:
            out += "CONTINUE";
            break;
        case TokenType::INT:
            out += "INT";
            break;
        case TokenType::DOUBLE:
            out += "DOUBLE";
            break;
        case TokenType::BOOL:
            out += "BOOL";
            break;
        case TokenType::STRING:
            out += "STRING";
            break;
        case TokenType::INT_LIT:
            out += "INT_LIT";
            break;
        case TokenType::DOUBLE_LIT:
            out += "DOUBLE_LIT";
            break;
        case TokenType::BOOL_LIT:
            out += "BOOL_LIT";
            break;
        case TokenType::STRING_LIT:
            out += "STRING_LIT";
            break;
        case TokenType::IDENTIFIER:
            out += "IDENTIFIER";
            break;
        case TokenType::RETURN:
            out += "RETURN";
            break;
        case TokenType::ERROR:
            out += "ERROR";
            break;
        case TokenType::EOFILE:
            out += "EOFILE";
            break;
    }
    return out;
}

bool isValidIdentifier(string str) {
    if (str[0] >= '0' && str[0] <= '9') return false;

    for (string s : keywords) {
        if (str == s) return false;
    }
    for (char c : str) {
        if ((!isalnum(c) || (c == '_')) || c == ';' || c == '\"' || c == ' ') return false;
    }
    return true;
}

string toString(const Token &tk) {
    TokenType type = tk.getType();
    string    out = toString(type);

    switch (type) {
        default:
            break;
        case TokenType::INT_LIT:
            out += ": " + std::to_string(tk.getInt());
            break;
        case TokenType::DOUBLE_LIT:
            out += ": " + std::to_string(tk.getDouble());
            break;
        case TokenType::BOOL_LIT:
            out += ": " + std::to_string(tk.getBool());
            break;
        case TokenType::STRING_LIT:
            out += ": \"" + tk.getString() + "\"";
            break;
        case TokenType::IDENTIFIER:
            out += ": " + tk.getName();
            break;
    }
    return out;
}

Token::Token(TokenType type) { this->type = type; }

int parseInt(string str) {
    if (str[0] == '+' || str[0] == '-') throw NumberFormatException("Invalid initial sign.");
    char *check;
    long  number;
    if (str.substr(0, 2) == "0x") {
        number = strtol(str.substr(2).c_str(), &check, 16);
    }
    else if (str.substr(0, 2) == "0b") {
        number = strtol(str.substr(2).c_str(), &check, 2);
    }

    else
        number = strtol(str.c_str(), &check, 10);

    if (check[0] != '\0') {
        throw NumberFormatException("Invalid int.");
    }

    if (number < 0) throw NumberFormatException("Negative.");

    return (int) number;
}

double parseDouble(string str) {
    if (str[0] == '+' || str[0] == '-') throw NumberFormatException("Invalid initial sign.");
    char  *check;
    double number = strtod(str.c_str(), &check);

    if (check[0] != '\0') {
        throw NumberFormatException("Invalid double.");
    }

    if (number < 0) throw NumberFormatException("Negative.");
    return number;
}

bool isStringLit(string token) {
    if (token.length() <= 1) return false;

    int quotesAmount = 0;
    for (unsigned int i = 0; i < token.length(); i++) {
        char c = token[i];
        if (c == '\"')
            quotesAmount++;
        else if (c == '\\' && token[i + 1] == '\"')
            quotesAmount--;
    }
    if (quotesAmount != 2) return false;
    if (token[0] == '\"' && token[token.length() - 1] == '\"') return true;
    return false;
}

void Token::formatEscapeSeqs() {
    auto   size = stringValue.size();
    string out;

    for (unsigned long i = 0; i < size; i++) {
        char c = stringValue[i];
        if (c == '\\' && i != size - 1) {
            switch (stringValue[i + 1]) {
                case 'a':
                    out += '\a';
                    break;
                case 'b':
                    out += '\b';
                    break;
                case 't':
                    out += '\t';
                    break;
                case 'n':
                    out += '\n';
                    break;
                case 'v':
                    out += '\v';
                    break;
                case 'f':
                    out += '\f';
                    break;
                case 'r':
                    out += '\r';
                    break;
                case '\\':
                    out += '\\';
                    break;
                case '\"':
                    out += '\"';
                    break;
                case '\'':
                    out += '\'';
                    break;
                default:
                    reportError("\\" + std::to_string(stringValue[i + 1]) + " is not a valid escape code");
                    type = TokenType::ERROR;
                    return;
            }
            i += 1;
        }
        else
            out += c;
    }
    stringValue = out;
}

Token::Token(string token) {
    if (isStringLit(token)) {
        type = TokenType::STRING_LIT;
        stringValue = token.substr(1, token.length() - 2);
        formatEscapeSeqs();
        return;
    }

    try {
        intValue = parseInt(token);
        type = TokenType::INT_LIT;
        return;
    } catch (NumberFormatException &ignored) {
    }

    try {
        doubleValue = parseDouble(token);
        type = TokenType::DOUBLE_LIT;
        return;
    } catch (NumberFormatException &ignored) {
    }

    if (token == "+") {
        type = TokenType::PLUS;
    }
    else if (token == "-") {
        type = TokenType::MINUS;
    }
    else if (token == "*") {
        type = TokenType::STAR;
    }
    else if (token == "/") {
        type = TokenType::SLASH;
    }
    else if (token == ",") {
        type = TokenType::COMMA;
    }
    else if (token == ".") {
        type = TokenType::DOT;
    }
    else if (token == "%") {
        type = TokenType::MOD;
    }
    else if (token == "=") {
        type = TokenType::ASSIGN;
    }
    else if (token == "==") {
        type = TokenType::EQ;
    }
    else if (token == "!=") {
        type = TokenType::NOT_EQ;
    }
    else if (token == "<") {
        type = TokenType::LT;
    }
    else if (token == ">") {
        type = TokenType::GT;
    }
    else if (token == "<=") {
        type = TokenType::LEQ;
    }
    else if (token == ">=") {
        type = TokenType::GEQ;
    }
    else if (token == ";") {
        type = TokenType::SEMICOLON;
    }
    else if (token == "&&" || token == "and") {
        type = TokenType::AND;
    }
    else if (token == "!" || token == "not") {
        type = TokenType::NOT;
    }
    else if (token == "||" || token == "or") {
        type = TokenType::OR;
    }
    else if (token == "(") {
        type = TokenType::LEFT_PAREN;
    }
    else if (token == ")") {
        type = TokenType::RIGHT_PAREN;
    }
    else if (token == "[") {
        type = TokenType::LEFT_SQR;
    }
    else if (token == "]") {
        type = TokenType::RIGHT_SQR;
    }
    else if (token == "{") {
        type = TokenType::LEFT_CUR;
    }
    else if (token == "}") {
        type = TokenType::RIGHT_CUR;
    }
    else if (token == "if") {
        type = TokenType::IF;
    }
    else if (token == "else") {
        type = TokenType::ELSE;
    }
    else if (token == "for") {
        type = TokenType::FOR;
    }
    else if (token == "while") {
        type = TokenType::WHILE;
    }
    else if (token == "break") {
        type = TokenType::BREAK;
    }
    else if (token == "continue") {
        type = TokenType::CONTINUE;
    }
    else if (token == "int") {
        type = TokenType::INT;
    }
    else if (token == "double") {
        type = TokenType::DOUBLE;
    }
    else if (token == "bool") {
        type = TokenType::BOOL;
    }
    else if (token == "string") {
        type = TokenType::STRING;
    }
    else if (token == "return") {
        type = TokenType::RETURN;
    }
    else if (token == "true" || token == "false") {
        type = TokenType::BOOL_LIT;
        boolValue = (token == "true") ? true : false;
    }
    else {
        if (isValidIdentifier(token)) {
            type = TokenType::IDENTIFIER;
            identifierName = token;
        }
        else {
            type = TokenType::ERROR;
        }
    }
}

Token::Token(const Token &other) {
    type = other.getType();
    switch (type) {
        case TokenType::INT_LIT:
            intValue = other.intValue;
            return;
        case TokenType::DOUBLE_LIT:
            doubleValue = other.doubleValue;
            return;
        case TokenType::BOOL_LIT:
            boolValue = other.boolValue;
            return;
        case TokenType::STRING_LIT:
            stringValue = other.stringValue;
            return;
        case TokenType::IDENTIFIER:
            identifierName = other.identifierName;
            return;
        default:
            return;
    }
}

TokenType Token::getType() const { return type; }

int       Token::getInt() const { return intValue; }

double    Token::getDouble() const { return doubleValue; }

bool      Token::getBool() const { return boolValue; }

string    Token::getString() const { return stringValue; }

string    Token::getName() const { return identifierName; }

bool      Token::isLiteral() const {
    switch (type) {
        case TokenType::INT_LIT:
        case TokenType::BOOL_LIT:
        case TokenType::DOUBLE_LIT:
        case TokenType::STRING_LIT:
            return true;
        default:
            return false;
    }
}

bool Token::isOperator() const {
    switch (type) {
        case TokenType::PLUS:
        case TokenType::MINUS:
        case TokenType::STAR:
        case TokenType::SLASH:
        case TokenType::MOD:
            return true;
        default:
            return false;
    }
}

void Token::nullify() { type = TokenType::EOFILE; }