#pragma once

using namespace std;

#include <string>

enum Terminal
{
    TYPE,
    BOOLEAN,
    WHILE,
    FOR,
    OUT,
    IF,
    ELSE,
    OPERATOR,
    EQUAL,
    CURLY_BRACKET_OPEN,
    CURLY_BRACKET_CLOSE,
    BARCKET_OPEN,
    BARCKET_CLOSE,
    SEMICOLON,
    IDENTIFIER,
    LITERAL,
    END
};

struct Token
{
    string token;
    int type;
} typedef Token;
