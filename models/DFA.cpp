using namespace std;

#include <string>
#include <fstream>

class DFA
{
private:
    int transitionTable[128][128];
    int initialState;

public:
    DFA();
    void init();
    void saveTransitionTable(string path);
    void loadTransitionTable(string path);
    void addToken(string token);
    int (*getTransitionTable())[128] { return this->transitionTable; }
    ~DFA();
};

DFA::DFA()
{
    this->initialState = 2;
}

void DFA::init()
{
    for (int i = 0; i < 128; i++)
    {
        if (isalnum(i) || i == '_' || isalpha(i))
        {
            this->transitionTable[0][i] = 1;
        }
        else
        {
            this->transitionTable[0][i] = -1;
        }
    }
    for (int i = 0; i < 128; i++)
    {
        if (isalnum(i) || i == '_' || isalpha(i))
        {
            this->transitionTable[1][i] = 1;
        }
        else
        {
            this->transitionTable[1][i] = -1;
        }
    }
    for (int i = 2; i < 128; i++)
    {
        for (int j = 0; j < 128; j++)
        {
            this->transitionTable[i][j] = -1;
        }
    }
}

void DFA::addToken(string token)
{
    if (this->transitionTable[0][token[0]] == -1)
    {
        this->transitionTable[0][token[0]] = this->initialState;
        this->initialState++;
    }
    int currentState = this->transitionTable[0][token[0]];
    for (int i = 1; i < token.length(); i++)
    {
        if (this->transitionTable[currentState][token[i]] == -1)
        {
            this->transitionTable[currentState][token[i]] = this->initialState;
            this->initialState++;
        }
        currentState = this->transitionTable[currentState][token[i]];
    }
}

void DFA::saveTransitionTable(string path)
{
    ofstream file;
    file.open(path);
    for (int i = 0; i < sizeof(this->transitionTable) / sizeof(this->transitionTable[0]); i++)
    {
        for (int j = 0; j < sizeof(this->transitionTable[i]) / sizeof(this->transitionTable[i][0]); j++)
        {
            file << this->transitionTable[i][j] << ",";
        }
        file << endl;
    }
    file.close();
}

void DFA::loadTransitionTable(string path)
{
    ifstream file;
    file.open(path);
    string line;
    int i = 0;
    while (getline(file, line))
    {
        int j = 0;
        string token = "";
        for (int k = 0; k < line.length(); k++)
        {
            if (line[k] == ',')
            {
                this->transitionTable[i][j] = stoi(token);
                token = "";
                j++;
            }
            else
            {
                token += line[k];
            }
        }
        i++;
    }
    file.close();
}

DFA::~DFA()
{
}
