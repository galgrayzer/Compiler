#include "./headers/ErrorHandler.hpp"

ErrorHandler::ErrorHandler()
{
}

/**
 * Handles the error message and exits the program.
 *
 * @param message The error message to be handled.
 */
void ErrorHandler::handle(string message)
{
    cout << message << endl;
    exit(1);
}

/**
 * Handles a lexical error.
 * 
 * @param token The token that caused the error.
 * @param line The line number where the error occurred.
 */
void ErrorHandler::lexicalError(string token, int line)
{
    handle("Lexical error: " + token + " at line: " + to_string(line));
}

/**
 * Handles a syntax error by displaying an error message with the token information.
 * @param token The token that caused the syntax error.
 */
void ErrorHandler::syntaxError(Token token)
{
    handle("Syntax error: " + token.token + " at line: " + to_string(token.line));
}

/**
 * Handles a semantic error.
 *
 * This function takes a message and a line number as parameters and handles a semantic error.
 * It constructs an error message by concatenating the given message and line number, and then
 * passes it to the `handle` function for further processing.
 *
 * @param message The error message.
 * @param line The line number where the error occurred.
 */
void ErrorHandler::SymanticError(string message, int line)
{
    handle("Symantic error: " + message + " at line: " + to_string(line));
}

ErrorHandler::~ErrorHandler()
{
}
