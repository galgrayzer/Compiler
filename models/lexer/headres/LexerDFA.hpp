#pragma once

using namespace std;

#define TRANSISTION_TABLE_SIZE 128

#include <string>
#include <fstream>

#include "../../headers/Token.hpp"

class DFA
{
private:
    int transitionTable[TRANSISTION_TABLE_SIZE][TRANSISTION_TABLE_SIZE]; // transition table
    int stateArray[TRANSISTION_TABLE_SIZE];                              // state array
    int initialState;                                                    // initial state
    void fillRowIdentifier(int row);                                     // fill row for identifier
    bool needIdentifierFill(int type);                                   // check if row needs to be filled for identifier

public:
    DFA();                                                                                // constructor
    void saveTransitionTable(string path);                                                // save transition table to file
    void loadTransitionTable(string path);                                                // load transition table from file
    void addToken(string token, int type);                                                // add token to the DFA
    int (*getTransitionTable())[TRANSISTION_TABLE_SIZE] { return this->transitionTable; } // get transition table
    int *getStateArray() { return this->stateArray; }                                     // get state array
    ~DFA();                                                                               // destructor
};