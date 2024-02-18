#include "./headers/ErrorHandler.hpp"

ErrorHandler::ErrorHandler()
{
}

void ErrorHandler::handle(string message)
{
    cout << message << endl;
    exit(1);
}

void ErrorHandler::lexicalError()
{
    handle("Lexical error");
}

ErrorHandler::~ErrorHandler()
{
}
