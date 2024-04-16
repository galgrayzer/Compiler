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
    handle("Lexical error: " + token + " at line: " + to_string(line));
}

void ErrorHandler::syntaxError(Token token)
{
    handle("Syntax error: " + token.token + " at line: " + to_string(token.line));
}

void ErrorHandler::SymanticError(string message, int line)
{
    handle("Symantic error: " + message + " at line: " + to_string(line));
}

ErrorHandler::~ErrorHandler()
{
}
