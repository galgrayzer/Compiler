using namespace std;

#include <iostream>

#include "./controllers/headers/ErrorHandler.hpp"
#include "./controllers/headers/LexicalAnalyzer.hpp"
#include "./controllers/headers/SyntacticAnalyzer.hpp"
#include "./controllers/headers/SymanticAnalyzer.hpp"

void printTokens(list<Token> tokens)
{
    for (Token t : tokens)
    {
        cout << "Token: " << t.token << " Type: " << t.type << endl;
    }
}

int main(int argc, char const *argv[])
{
    const char *FILE_PATH = "main.gal"; // argv[1];

    ErrorHandler *error = new ErrorHandler(); // Error handler

    LexicalAnalyzer *lex = new LexicalAnalyzer((char *)FILE_PATH, error); // Lexical analyzer
    list<Token> tokens = lex->lexer();                                    // List of tokens

    SyntacticAnalyzer *syn = new SyntacticAnalyzer(tokens, error);                          // Syntactic analyzer
    AST *ast = syn->parser("./DFAs/parser/parserDFA.txt", "./DFAs/parser/parserRules.txt"); // Abstract syntax tree

    SymanticAnalyzer *sym = new SymanticAnalyzer(ast, error); // Symantic analyzer
    sym->printSymbolTable();                                  // Print symbol table
    ast = sym->symantic();                                    // Symantic analysis

    cout << endl
         << "AST: " << endl;
    ast->print();

    return 0;
}