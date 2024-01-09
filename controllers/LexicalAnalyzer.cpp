using namespace std;

#define DFA_PATH "./DFAs/lexer.txt"

#include <fstream>
#include <list>
#include <unordered_map>
#include <regex>
#include <iostream>

#include "../models/Token.cpp"
#include "../models/DFA.cpp"

class LexicalAnalyzer
{
private:
    // Attributes
    char *filePath;
    DFA *dfa;
    const unordered_map<string, string> DEFINED_VALUES = {
        {"+", "OPERATOR"},
        {"++", "OPERATOR"},
        {"-", "OPERATOR"},
        {"--", "OPERATOR"},
        {"*", "OPERATOR"},
        {"/", "OPERATOR"},
        {"%", "OPERATOR"},
        {"=", "OPERATOR"},
        {"==", "OPERATOR"},
        {"!=", "OPERATOR"},
        {"<", "OPERATOR"},
        {">", "OPERATOR"},
        {"<=", "OPERATOR"},
        {">=", "OPERATOR"},
        {"&&", "OPERATOR"},
        {"||", "OPERATOR"},
        {"!", "OPERATOR"},
        {"++", "OPERATOR"},
        {"--", "OPERATOR"},
        {"(", "SEPERATOR"},
        {")", "SEPERATOR"},
        {"{", "SEPERATOR"},
        {"}", "SEPERATOR"},
        {";", "SEPERATOR"},
        {"int", "KEYWORD"},
        {"char", "KEYWORD"},
        {"bool", "KEYWORD"},
        {"if", "KEYWORD"},
        {"else", "KEYWORD"},
        {"while", "KEYWORD"},
        {"for", "KEYWORD"},
        {"true", "KEYWORD"},
        {"false", "KEYWORD"},
        {"out", "KEYWORD"}};

    // Methods
    Token *tokenizer(string token);
    string getToken(string line);

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

// string LexicalAnalyzer::getToken(string line)
// {
// }

list<Token> LexicalAnalyzer::lexer()
{
    list<Token> tokens;
    ifstream file(this->filePath);
    string line;
    while (getline(file, line))
    {
        string token = "";
        Token *t;
        int currentState = 0;
        for (int i = 0; i < line.length(); i++)
        {
            if (this->dfa->getTransitionTable()[currentState][line[i]] == -1)
            {
                continue;
            }
            if (this->dfa->getTransitionTable()[currentState][line[i]] != -1)
            {
                while (this->dfa->getTransitionTable()[currentState][line[i]] != -1)
                {
                    token += line[i];
                    currentState = this->dfa->getTransitionTable()[currentState][line[i]];
                    i++;
                }
                i--;
            }
            t = tokenizer(token);
            tokens.push_back(*t);
            currentState = 0;
            token = "";
        }
    }
    file.close();
    return tokens;
}

Token *LexicalAnalyzer::tokenizer(string token)
{
    Token *t = new Token();
    t->token = token;
    if (this->DEFINED_VALUES.find(token) != this->DEFINED_VALUES.end())
    {
        t->type = this->DEFINED_VALUES.at(token);
    }
    else if (regex_match(token, regex("[0-9]+")) || regex_match(token, regex("'.'")))
    {
        t->type = "LITERAL";
    }
    else
    {
        t->type = "IDENTIFIER";
    }
    return t;
}

LexicalAnalyzer::~LexicalAnalyzer()
{
    delete this->dfa;
}
