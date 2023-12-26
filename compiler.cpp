using namespace std;

#include <iostream>

#include "./controllers/LexicalAnalyzer.cpp"

void printTokens(list<Token> tokens)
{
    for (Token t : tokens)
    {
        cout << "Token: " << t.token << " Type: " << t.type << endl;
    }
}

int main(int argc, char const *argv[])
{
    const char *FILE_PATH = "main.gal"; //argv[1];
    LexicalAnalyzer *lex = new LexicalAnalyzer((char *)FILE_PATH);
    list<Token> tokens = lex->lexer();
    printTokens(tokens);
    return 0;
}
