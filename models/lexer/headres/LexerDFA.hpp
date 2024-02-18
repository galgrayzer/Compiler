#pragma once

using namespace std;

#define TRANSISTION_TABLE_SIZE 128

#include <string>
#include <fstream>

#include "../../headers/Token.hpp"

class DFA
{
private:
    int transitionTable[TRANSISTION_TABLE_SIZE][TRANSISTION_TABLE_SIZE];
    int stateArray[TRANSISTION_TABLE_SIZE];
    int initialState;
    void fillRowIdentifier(int row);

public:
    DFA();
    void saveTransitionTable(string path);
    void loadTransitionTable(string path);
    void addToken(string token, int type);
    int (*getTransitionTable())[TRANSISTION_TABLE_SIZE] { return this->transitionTable; }
    int *getStateArray() { return this->stateArray; }
    ~DFA();
};