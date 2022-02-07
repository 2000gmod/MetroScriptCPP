#include "linepr.hpp"

#include <algorithm>

using std::string;
using std::vector;

string preProcessString(string line);
string removeComments(string line);
vector<Token> tokenize(string line);
string removeLeadingSpaces(string line);

vector<string> getFileLines(string fileContent) {
    string modLine = preProcessString(fileContent);
    string nline = "\n";
    size_t pos = 0;
    string line;
    vector<string> out;

    while ((pos = modLine.find(nline)) != std::string::npos) {
        line = modLine.substr(0, pos);
        out.push_back(line);
        modLine.erase(0, pos + nline.length());
    }
    out.push_back(modLine);
    return out;
}

vector<Token> tokenizeString(string input) {
    vector<Token> out = tokenize(input);
    out.push_back(Token(TokenType::EOFILE));

    return out;
}

int countLeadingNewLines(const string &line) {
    int out = 0;
    for (const char c : line) {
        if (c == '\n')
            out++;
        else
            break;
    }
    return out;
}

vector<Token> tokenize(string line) {
    string modifyLine = preProcessString(line);

    vector<Token> out;
    string greatestValidToken, tokenToTry;
    int lineNum = 0;

    while (!modifyLine.empty()) {
        lineNum += countLeadingNewLines(modifyLine);
        modifyLine = removeLeadingSpaces(modifyLine);
        greatestValidToken = "";
        tokenToTry = "";

        unsigned short counter = 0;
        for (char c : modifyLine) {
            if (counter >= MAX_SCANNER_RANGE) break;
            if (c >= 32)
                tokenToTry += c;
            else
                continue;

            Token tokenTest(tokenToTry);
            if (tokenTest.getType() != TokenType::ERROR) {
                greatestValidToken = tokenToTry;

                if (greatestValidToken[0] != '\"' && greatestValidToken[greatestValidToken.length() - 1] != '\"') {
                    std::string::iterator endPos =
                        std::remove(greatestValidToken.begin(), greatestValidToken.end(), ' ');
                    greatestValidToken.erase(endPos, greatestValidToken.end());
                }
            }
            counter++;
        }

        if (!greatestValidToken.empty()) {
            out.push_back(Token(greatestValidToken, lineNum));
            modifyLine = modifyLine.substr(greatestValidToken.length());
        }
        else {
            out.push_back(Token("?", lineNum));
            break;
        }
    }
    return out;
}

string preProcessString(string line) {
    line = removeComments(line);
    return line;
}

string removeComments(string line) {
    string out;
    bool writing = true;
    auto size = line.size();

    const string beginComment = "/*";
    const string endComment = "*/";

    if (beginComment != endComment) {
        for (unsigned long i = 0; i < size; i++) {
            if (line.substr(i, beginComment.size()) == beginComment)
                writing = false;
            else if (line.substr(i, endComment.size()) == endComment) {
                writing = true;
                i += endComment.size();
            }
            if (writing) out += line[i];
        }
    }
    else {
        for (unsigned long i = 0; i < size; i++) {
            if (line.substr(i, beginComment.size()) == beginComment) {
                writing = !writing;
                i += beginComment.size();
            }
            if (writing) out += line[i];
        }
    }
    return out;
}

string removeLeadingSpaces(string line) {
    string out;
    bool writing = false;
    for (char c : line) {
        if (c > 32) writing = true;
        if (writing) out += c;
    }
    return out;
}