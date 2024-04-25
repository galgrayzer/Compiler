#pragma once

#include "../../headers/Token.hpp"
#include "./Gramer.hpp"
#include <fstream>
#include <list>
#include <iostream>

#define TERMINAL 17      // 17 terminals
#define NONE_TERMINAL 11 // 11 none terminals
#define STATES 99        // 99 states

enum NonTerminal
{
    PROGRAM = TERMINAL,
    STATEMENT_LIST,
    STATEMENT,
    DECLARATION,
    ASSIGNMENT,
    EXPRESSION,
    TERM,
    CONDITIONAL,
    CONDITION,
    LOOP,
    OUTFUNC
};

enum Action
{
    SHIFT = 100,
    REDUCE = 200,
    ACCEPT = 300,
    ERROR = 400
};

class ParserDFA
{
private:
    int parserDFA[STATES][TERMINAL + NONE_TERMINAL]; // parser table
    int initialState;                                // initial state
    int followSet[NONE_TERMINAL][TERMINAL];          // follow set
    int firstSet[NONE_TERMINAL][TERMINAL];           // first set
    int becomeSet[NONE_TERMINAL][NONE_TERMINAL];     // become set
    list<Gramer> *grammarRules;                      // grammar rules

    void startingPhase(int final);             // starting phase of the parser
    void fillExpansions(int state, int final); // fill expansions

public:
    string ENG_STRINGS[NONE_TERMINAL] = {"PROGRAM", "STATEMENT_LIST", "STATEMENT", "DECLARATION", "ASSIGNMENT", "EXPRESSION", "TERM", "CONDITIONAL", "CONDITION", "LOOP", "OUTFUNC"};
    ParserDFA();                                                                   // constructor
    void AddGrammar(Gramer gramer);                                                // add grammar
    Gramer *getGrammarRules();                                                     // get grammar rules
    void buildTable(int final);                                                    // build the parser table
    void saveTable(string tablePath, string setsPath, string rulesPath);           // save the parser table
    void loadTable(string tablePath, string rulesPath);                            // load the parser table
    void savePrettyTable(string tablePath);                                        // save pretty table
    int (*getParsingTable())[TERMINAL + NONE_TERMINAL] { return this->parserDFA; } // get parsing table
    ~ParserDFA();                                                                  // destructor
};