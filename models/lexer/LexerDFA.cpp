using namespace std;

#define TRANSISTION_TABLE_SIZE 128

#include <string>
#include <fstream>

#include "../Token.cpp"

class DFA
{
private:
    int transitionTable[TRANSISTION_TABLE_SIZE][TRANSISTION_TABLE_SIZE];
    int stateArray[TRANSISTION_TABLE_SIZE];
    int initialState;

public:
    DFA();
    void saveTransitionTable(string path);
    void loadTransitionTable(string path);
    void addToken(string token, int type);
    int (*getTransitionTable())[TRANSISTION_TABLE_SIZE] { return this->transitionTable; }
    int *getStateArray() { return this->stateArray; }
    ~DFA();
};

DFA::DFA()
{
    this->initialState = 1;
    for (int i = 0; i < TRANSISTION_TABLE_SIZE - 1; i++)
    {
        for (int j = 0; j < TRANSISTION_TABLE_SIZE; j++)
        {
            if ((isalpha(j) || j == '_' || isdigit(j)))
                this->transitionTable[i][j] = TRANSISTION_TABLE_SIZE - 2;
            else
                this->transitionTable[i][j] = -1;
        }
        if ((isdigit(i)))
        {
            this->transitionTable[0][i] = TRANSISTION_TABLE_SIZE - 1;
            this->transitionTable[TRANSISTION_TABLE_SIZE - 1][i] = TRANSISTION_TABLE_SIZE - 1;
        }
        else
            transitionTable[TRANSISTION_TABLE_SIZE - 1][i] = -1;
        this->stateArray[i] = IDENTIFIER;
    }
    this->stateArray[TRANSISTION_TABLE_SIZE - 1] = LITERAL;
}

void DFA::addToken(string token, int type)
{
    if (this->transitionTable[0][token[0]] == -1 || this->transitionTable[0][token[0]] == TRANSISTION_TABLE_SIZE - 2)
    {
        this->transitionTable[0][token[0]] = this->initialState;
        this->initialState++;
    }
    int currentState = this->transitionTable[0][token[0]];
    for (int i = 1; i < token.length(); i++)
    {
        if (this->transitionTable[currentState][token[i]] == -1 || this->transitionTable[currentState][token[i]] == TRANSISTION_TABLE_SIZE - 2)
        {
            this->transitionTable[currentState][token[i]] = this->initialState;
            this->initialState++;
        }
        currentState = this->transitionTable[currentState][token[i]];
    }
    this->stateArray[initialState - 1] = type;
}

void DFA::saveTransitionTable(string path)
{
    ofstream file;
    file.open(path);
    for (int i = 0; i < TRANSISTION_TABLE_SIZE; i++)
    {
        for (int j = 0; j < TRANSISTION_TABLE_SIZE; j++)
        {
            file << this->transitionTable[i][j] << ",";
        }
        file << endl;
    }
    for (int i = 0; i < TRANSISTION_TABLE_SIZE; i++)
    {
        file << this->stateArray[i] << ",";
    }
    file.close();
}

void DFA::loadTransitionTable(string path)
{
    ifstream file;
    file.open(path);
    string line;
    int i, k, j = 0;
    string token = "";
    for (i = 0; i < TRANSISTION_TABLE_SIZE; i++)
    {
        getline(file, line);
        for (k = 0; k < line.length(); k++)
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
    }
    getline(file, line);
    j = 0;
    string state = "";
    for (i = 0; i < line.length(); i++)
    {
        if (line[i] == ',')
        {
            this->stateArray[j] = stoi(state);
            state = "";
            j++;
        }
        else
        {
            state += line[i];
        }
    }
    file.close();
}

DFA::~DFA()
{
}
