#pragma once

using namespace std;

#define DFA_PATH "./DFAs/lexer/lexerDFA.txt"

#include <fstream>
#include <list>
#include <iostream>

#include "../../models/headers/Token.hpp"
#include "../../models/lexer/headres/LexerDFA.hpp"
#include "ErrorHandler.hpp"

class LexicalAnalyzer
{
private:
    // Attributes
    char *filePath;      // path of the file to be analyzed
    DFA *dfa;            // DFA for lexical analysis
    ErrorHandler *error; // error handler

    // Methods
    Token *tokenizer(string token, int state, int line); // tokenizes the lexeme

public:
    // Constructor and Destructor
    LexicalAnalyzer(char *path, ErrorHandler *error); // constructor
    ~LexicalAnalyzer();                               // destructor

    // Methods
    list<Token> lexer(); // performs lexical analysis
};