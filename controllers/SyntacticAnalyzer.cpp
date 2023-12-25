#include <list>

#include "../models/Token.cpp"
#include "../models/AST.cpp"

class SyntacticAnalyzer
{
private:
    // Arributes
    list<Token> tokens;

public:
    // Constructor and Destructor
    SyntacticAnalyzer(list<Token> tokens);
    ~SyntacticAnalyzer();
};

SyntacticAnalyzer::SyntacticAnalyzer(list<Token> tokens)
{
    this->tokens = tokens;
}

SyntacticAnalyzer::~SyntacticAnalyzer()
{
}
