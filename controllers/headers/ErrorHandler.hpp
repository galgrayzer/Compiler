#pragma once

using namespace std;

#include <iostream>
#include <exception>

class ErrorHandler
{
private:
    void handle(string message);

public:
    ErrorHandler();
    void lexicalError(string token, int line);
    ~ErrorHandler();
};