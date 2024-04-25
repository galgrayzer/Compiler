#pragma once

using namespace std;

#include <string>

enum Terminal
{
    TYPE,
    IDENTIFIER,
    SEMICOLON,
    EQUAL,
    OPERATOR,
    LITERAL,
    BARCKET_OPEN,
    BARCKET_CLOSE,
    IF,
    CURLY_BRACKET_OPEN,
    CURLY_BRACKET_CLOSE,
    ELSE,
    BOOLEAN,
    WHILE,
    FOR,
    OUT,
    END
};

struct Token
{
    string token;      // token
    int type;          // type of the token
    int line;          // line number
    int typeCode = -1; // type code of the token
} typedef Token;
