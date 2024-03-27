#include "./headres/LexerDFA.hpp"

DFA::DFA()
{
    this->initialState = 1;
    for (int i = 0; i < TRANSISTION_TABLE_SIZE; i++)
    {
        if ((isalpha(i) || i == '_'))
        {
            this->transitionTable[0][i] = TRANSISTION_TABLE_SIZE - 2;
            this->transitionTable[TRANSISTION_TABLE_SIZE - 2][i] = TRANSISTION_TABLE_SIZE - 2;
            this->transitionTable[TRANSISTION_TABLE_SIZE - 1][i] = -2;
        }
        else if (isdigit(i))
        {
            this->transitionTable[0][i] = TRANSISTION_TABLE_SIZE - 1;
            this->transitionTable[TRANSISTION_TABLE_SIZE - 1][i] = TRANSISTION_TABLE_SIZE - 1;
            this->transitionTable[TRANSISTION_TABLE_SIZE - 2][i] = TRANSISTION_TABLE_SIZE - 2;
        }
        else
        {
            this->transitionTable[0][i] = -1;
            this->transitionTable[TRANSISTION_TABLE_SIZE - 2][i] = -1;
            this->transitionTable[TRANSISTION_TABLE_SIZE - 1][i] = -1;
        }
    }
    for (int i = 1; i < TRANSISTION_TABLE_SIZE - 2; i++)
    {
        for (int j = 0; j < TRANSISTION_TABLE_SIZE; j++)
        {
            this->transitionTable[i][j] = -1;
        }
        this->stateArray[i] = IDENTIFIER;
    }
    this->stateArray[TRANSISTION_TABLE_SIZE - 2] = IDENTIFIER;
    this->stateArray[TRANSISTION_TABLE_SIZE - 1] = LITERAL;
}

void DFA::fillRowIdentifier(int row)
{
    for (int i = 0; i < TRANSISTION_TABLE_SIZE; i++)
    {
        if ((isalpha(i) || i == '_' || isdigit(i)) && (this->transitionTable[row][i] == -1))
        {
            this->transitionTable[row][i] = TRANSISTION_TABLE_SIZE - 2;
        }
    }
}

void DFA::addToken(string token, int type)
{
    if (this->transitionTable[0][token[0]] == -1 || this->transitionTable[0][token[0]] == TRANSISTION_TABLE_SIZE - 2)
    {
        this->transitionTable[0][token[0]] = this->initialState;
        if (type < OPERATOR)
        {
            this->fillRowIdentifier(initialState);
        }
        this->initialState++;
    }
    int currentState = this->transitionTable[0][token[0]];
    for (int i = 1; i < token.length(); i++)
    {
        if (this->transitionTable[currentState][token[i]] == -1 || this->transitionTable[currentState][token[i]] == TRANSISTION_TABLE_SIZE - 2)
        {
            this->transitionTable[currentState][token[i]] = this->initialState;
            this->initialState++;
            if (type < OPERATOR)
            {
                this->fillRowIdentifier(initialState);
            }
        }
        currentState = this->transitionTable[currentState][token[i]];
    }
    this->stateArray[currentState] = type;
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
        k = 0;
        for (j = 0; j < line.length(); j++)
        {
            if (line[j] == ',')
            {
                this->transitionTable[i][k] = stoi(token);
                token = "";
                k++;
            }
            else
            {
                token += line[j];
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
