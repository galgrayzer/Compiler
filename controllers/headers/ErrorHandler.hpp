#pragma once

using namespace std;

#include <iostream>
#include <exception>
#include "../../models/headers/Token.hpp"

class ErrorHandler
{
private:
    void handle(string message); // handle error

public:
    ErrorHandler();                               // constructor
    void lexicalError(string token, int line);    // handle lexical error
    void syntaxError(Token token);                // handle syntax error
    void SymanticError(string message, int line); // handle symantic error
    ~ErrorHandler();                              // destructor
};