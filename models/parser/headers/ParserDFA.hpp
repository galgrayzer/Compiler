#pragma once

#include "../../headers/Token.hpp"
#include "./Gramer.hpp"
#include <fstream>
#include <list>
#include <set>
#include <iostream>

#define TERMINAL 17
#define NONE_TERMINAL 11
#define STATES 99

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
};

class ParserDFA
{
private:
    int parserDFA[STATES][TERMINAL + NONE_TERMINAL];
    int initialState;
    int followSet[NONE_TERMINAL][TERMINAL];
    int firstSet[NONE_TERMINAL][TERMINAL];
    int becomeSet[NONE_TERMINAL][NONE_TERMINAL];
    list<Gramer> *grammarRules;

    list<Gramer> *closure(list<Gramer> *i, int handled[NONE_TERMINAL] = NULL);

public:
    ParserDFA();
    void AddGrammar(Gramer gramer);
    Gramer *getGrammarRules();
    void buildTable(int final);
    void saveTable(string tablePath, string setsPath, string rulesPath);
    void loadTable(string tablePath, string rulesPath);
    void savePrettyTable(string tablePath);
    int (*getParsingTable())[TERMINAL + NONE_TERMINAL] { return this->parserDFA; }
    ~ParserDFA();
};