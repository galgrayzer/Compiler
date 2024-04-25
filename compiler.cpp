using namespace std;

#include <iostream>

#include "./controllers/headers/ErrorHandler.hpp"
#include "./controllers/headers/LexicalAnalyzer.hpp"
#include "./controllers/headers/SyntacticAnalyzer.hpp"
#include "./controllers/headers/SymanticAnalyzer.hpp"
#include "./controllers/headers/CodeGeneration.hpp"

int main(int argc, char const *argv[])
{
    // check if arguments are provided
    if (argc != 3)
    {
        cout << "Usage: ./compiler <file_path> <output_file>" << endl;
        return 1;
    }

    const char *FILE_PATH = argv[1];   // File path
    const char *OUTPUT_FILE = argv[2]; // Output file

    ErrorHandler *error = new ErrorHandler(); // Error handler

    LexicalAnalyzer *lex = new LexicalAnalyzer((char *)FILE_PATH, error); // Lexical analyzer
    list<Token> tokens = lex->lexer();                                    // List of tokens

    SyntacticAnalyzer *syn = new SyntacticAnalyzer(tokens, error);                          // Syntactic analyzer
    AST *ast = syn->parser("./DFAs/parser/parserDFA.txt", "./DFAs/parser/parserRules.txt"); // Abstract syntax tree

    SymanticAnalyzer *sym = new SymanticAnalyzer(ast, error); // Symantic analyzer
    ast = sym->symantic();                                    // Symantic analysis

    CodeGeneration *code = new CodeGeneration(ast, sym->getSymbolTable()); // Code generation
    code->generate((string)OUTPUT_FILE);                                   // Generate code

    return 0; // Exit
}