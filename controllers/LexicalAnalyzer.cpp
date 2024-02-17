using namespace std;

#define DFA_PATH "./DFAs/lexer.txt"

#include <fstream>
#include <list>
#include <regex>
#include <iostream>

#include "../models/Token.cpp"
#include "../models/lexer/LexerDFA.cpp"

class LexicalAnalyzer
{
private:
    // Attributes
    char *filePath;
    DFA *dfa;

    // Methods
    Token *tokenizer(string token, int state);

public:
    // Constructor and Destructor
    LexicalAnalyzer(char *path);
    ~LexicalAnalyzer();

    // Methods
    list<Token> lexer();
};

LexicalAnalyzer::LexicalAnalyzer(char *path)
{
    this->filePath = path;
    this->dfa = new DFA();
    this->dfa->loadTransitionTable(DFA_PATH);
}

list<Token> LexicalAnalyzer::lexer()
{
    list<Token> tokens;
    ifstream file(this->filePath);
    string line, token = "";
    int(*transitionTable)[128] = this->dfa->getTransitionTable();
    while (getline(file, line))
    {
        Token *t;
        int currentState = 0;
        for (int i = 0; i < line.length(); i++)
        {
            if (transitionTable[currentState][line[i]] == -1)
            {
                continue;
            }
            if (transitionTable[currentState][line[i]] != -1)
            {
                while (transitionTable[currentState][line[i]] != -1)
                {
                    token += line[i];
                    currentState = transitionTable[currentState][line[i]];
                    i++;
                }
                i--;
            }
            t = tokenizer(token, currentState);
            tokens.push_back(*t);
            currentState = 0;
            token = "";
        }
    }
    file.close();
    return tokens;
}

Token *LexicalAnalyzer::tokenizer(string token, int state)
{
    Token *t = new Token();
    t->token = token;
    switch (this->dfa->getStateArray()[state])
    {
    case KEYWORD:
        t->type = "KEYWORD";
        break;
    case OPERATOR:
        t->type = "OPERATOR";
        break;
    case SEPERATOR:
        t->type = "SEPERATOR";
        break;
    case IDENTIFIER:
        t->type = "IDENTIFIER";
        break;
    case LITERAL:
        t->type = "LITERAL";
        break;
    }
    return t;
}

LexicalAnalyzer::~LexicalAnalyzer()
{
    delete this->dfa;
}
