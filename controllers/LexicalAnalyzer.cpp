#include "./headers/LexicalAnalyzer.hpp"

LexicalAnalyzer::LexicalAnalyzer(char *path, ErrorHandler *error)
{
    this->error = error;
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
    int lineCount = 0;
    while (getline(file, line))
    {
        lineCount++;
        Token *t;
        int currentState = 0;
        for (int i = 0; i < line.length(); i++)
        {
            if (transitionTable[currentState][line[i]] != -1)
            {
                while (transitionTable[currentState][line[i]] != -1)
                {
                    token += line[i];
                    currentState = transitionTable[currentState][line[i]];
                    if (currentState == -2)
                        this->error->lexicalError(token, lineCount);
                    i++;
                }
                i--;
                t = tokenizer(token, currentState);
                tokens.push_back(*t);
                currentState = 0;
                token = "";
            }
        }
    }
    file.close();
    return tokens;
}

Token *LexicalAnalyzer::tokenizer(string token, int state)
{
    Token *t = new Token();
    t->token = token;
    t->type = this->dfa->getStateArray()[state];
    return t;
}

LexicalAnalyzer::~LexicalAnalyzer()
{
    delete this->dfa;
}
