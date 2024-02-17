using namespace std;

#define TRANSISTION_TABLE_SIZE 128

#include <string>
#include <fstream>

class DFA
{
private:
    int transitionTable[TRANSISTION_TABLE_SIZE][TRANSISTION_TABLE_SIZE];
    string stateArray[TRANSISTION_TABLE_SIZE];
    int initialState;

public:
    DFA();
    void saveTransitionTable(string path);
    void loadTransitionTable(string path);
    void addToken(string token, string type);
    int (*getTransitionTable())[TRANSISTION_TABLE_SIZE] { return this->transitionTable; }
    string *getStateArray() { return this->stateArray; }
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
    }
    for (int i = 0; i < TRANSISTION_TABLE_SIZE; i++)
    {
        if ((isalpha(i)))
        {
            this->transitionTable[0][i] = TRANSISTION_TABLE_SIZE - 1;
            this->transitionTable[TRANSISTION_TABLE_SIZE - 1][i] = TRANSISTION_TABLE_SIZE - 1;
        }
        else
            transitionTable[TRANSISTION_TABLE_SIZE - 1][i] = -1;
    }
    this->stateArray[TRANSISTION_TABLE_SIZE - 1] = "LITERAL";
    this->stateArray[TRANSISTION_TABLE_SIZE - 2] = "IDENTIFIER";
}

void DFA::addToken(string token, string type)
{
    if (this->transitionTable[0][token[0]] == -1 || this->transitionTable[0][token[0]] == TRANSISTION_TABLE_SIZE - 1)
    {
        this->transitionTable[0][token[0]] = this->initialState;
        this->initialState++;
    }
    int currentState = this->transitionTable[0][token[0]];
    for (int i = 1; i < token.length(); i++)
    {
        if (this->transitionTable[currentState][token[i]] == -1 || this->transitionTable[currentState][token[i]] == TRANSISTION_TABLE_SIZE - 1)
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
    int temp = 0;
    for (int i = 0; i < TRANSISTION_TABLE_SIZE; i++)
    {
        getline(file, line);
        temp++;
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
    getline(file, line);
    int j = 0;
    string token = "";
    for (int k = 0; k < line.length(); k++)
    {
        if (line[k] == ',')
        {
            this->stateArray[j] = token;
            token = "";
            j++;
        }
        else
        {
            token += line[k];
        }
    }
    file.close();
}

DFA::~DFA()
{
}
