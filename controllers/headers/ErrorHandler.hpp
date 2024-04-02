#pragma once

using namespace std;

#include <iostream>
#include <exception>
#include "../../models/headers/Token.hpp"

class ErrorHandler
{
private:
    void handle(string message);

public:
    ErrorHandler();
    void lexicalError(string token, int line);
    void syntaxError(Token token);
    ~ErrorHandler();
};