using namespace std;

#include <string>
#include <fstream>

class DFA
{
private:
public:
    int transitionTable[7][128]; // 128 ASCII characters
    DFA();
    void addToken(string token);
    void removeToken(string token);
    void saveToFile(string filename);
    void loadFromFile(string filename);
    ~DFA();
};

DFA::DFA()
{
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 128; j++)
        {
            this->transitionTable[i][j] = -1;
        }
    }
}

void DFA::addToken(string token)
{
    int state = 0;
    for (int i = 0; i < token.length(); i++)
    {
        int ascii = (int)token[i];
        if (this->transitionTable[state][ascii] == -1)
        {
            this->transitionTable[state][ascii] = i + 1;
        }
        state = this->transitionTable[state][ascii];
    }
}

void DFA::removeToken(string token)
{
    int state = 0;
    for (int i = 0; i < token.length(); i++)
    {
        int ascii = (int)token[i];
        if (this->transitionTable[state][ascii] == -1)
        {
            return;
        }
        state = this->transitionTable[state][ascii];
    }
    this->transitionTable[state][token[token.length() - 1]] = -1;
}

void DFA::saveToFile(string filename)
{
    ofstream file;
    file.open(filename);
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 128; j++)
        {
            file << this->transitionTable[i][j] << ",";
        }
        file << endl;
    }
    file.close();
}

void DFA::loadFromFile(string filename)
{
    ifstream file;
    file.open(filename);
    string line;
    while (getline(file, line))
    {
        int startIndex = 0;
        int row = 0;
        int column = 0;
        for (int i = 0; i < line.length(); i++)
        {
            if (line[i] == ',')
            {
                string number = line.substr(startIndex, i - startIndex);
                startIndex = i + 1;
                this->transitionTable[row++][column] = stoi(number);
            }
            column++;
        }
    }
    file.close();
}

DFA::~DFA()
{
}