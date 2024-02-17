#ifndef TOKEN_H
#define TOKEN_H

using namespace std;

#include <string>

enum TokenType
{
    KEYWORD,
    OPERATOR,
    SEPERATOR,
    IDENTIFIER,
    LITERAL
};

struct Token
{
    string token;
    string type;
} typedef Token;
#endif
