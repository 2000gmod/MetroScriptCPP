#include "linepr.hpp"

#include <algorithm>

using std::string;
using std::vector;

string         preProcessString(string line);
string         removeComments(string line);
vector<string> preTokenize(string line);

vector<Token>  tokenizeString(string input) {
    vector<Token>  out;
    vector<string> preTokens = preTokenize(input);

    for (string strToken : preTokens) {
        out.push_back(Token(strToken));
    }

    out.push_back(Token(TokenType::EOFILE));

    return out;
}

vector<string> preTokenize(string line) {
    string         modifyLine = preProcessString(line);

    vector<string> out;
    string         greatestValidToken, tokenToTry;

    while (!modifyLine.empty()) {
        if (modifyLine[0] == ' ') modifyLine = modifyLine.substr(1);
        greatestValidToken = "";
        tokenToTry = "";

        for (char c : modifyLine) {
            if (c >= 32)
                tokenToTry += c;
            else
                continue;

            Token test1(tokenToTry);
            if (test1.getType() != TokenType::ERROR) {
                greatestValidToken = tokenToTry;

                if (greatestValidToken[0] != '\"' && greatestValidToken[greatestValidToken.length() - 1] != '\"') {
                    std::string::iterator endPos =
                        std::remove(greatestValidToken.begin(), greatestValidToken.end(), ' ');
                    greatestValidToken.erase(endPos, greatestValidToken.end());
                }
            }
        }

        if (!greatestValidToken.empty()) {
            out.push_back(greatestValidToken);
            modifyLine = modifyLine.substr(greatestValidToken.length());
        }
        else {
            out.push_back("?");
            break;
        }
    }
    return out;
}

string preProcessString(string line) {
    line = removeComments(line);
    string out;
    bool   writing = false;
    bool   isInsideString = false;

    for (char c : line) {
        writing = c != ' ';
        if (c == '\"') isInsideString = !isInsideString;
        if (writing || isInsideString) {
            out += c;
        }
    }
    return out;
}

string removeComments(string line) {
    string       out;
    bool         writing = true;
    auto         size = line.size();

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