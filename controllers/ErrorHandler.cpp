#include "./headers/ErrorHandler.hpp"

ErrorHandler::ErrorHandler()
{
}

void ErrorHandler::handle(string message)
{
    cout << message << endl;
    exit(1);
}

void ErrorHandler::lexicalError(string token, int line)
{
    handle("Invalid token: " + token + " at line: " + to_string(line));
}

void ErrorHandler::syntaxError(string token)
{
    handle("Syntax error: " + token);
}

ErrorHandler::~ErrorHandler()
{
}
