#include <list>

#include "../models/Token.cpp"
#include "../models/AST.cpp"
#include "../models/parser/Stack.cpp"

class SyntacticAnalyzer
{
private:
    list<Token> tokens;

public:
    SyntacticAnalyzer(list<Token> tokens);
    int parser();
    ~SyntacticAnalyzer();
};

SyntacticAnalyzer::SyntacticAnalyzer(list<Token> tokens)
{
    this->tokens = tokens;
}

bool dolarpop(string token)
{
    return token == "$";
}

int SyntacticAnalyzer::parser()
{
    Token *token = new Token();
    token->type = "PROGRAM";
    token->token = "PROGRAM";
    AST *abstractSyntaxTree = new AST(token);
    Stack<string> stack(100);
    stack.push("$");
    for (Token token : this->tokens)
    {
    }
    return 0;
}

SyntacticAnalyzer::~SyntacticAnalyzer()
{
}
