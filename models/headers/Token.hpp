#pragma once

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
