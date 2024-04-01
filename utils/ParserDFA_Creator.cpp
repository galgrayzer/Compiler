#include "../models/parser/headers/ParserDFA.hpp"
#include "../models/headers/Token.hpp"

#include <iostream>
#include <list>

int main(int argc, char const *argv[])
{
    ParserDFA *parserDFA = new ParserDFA();
    list<int> *right = new list<int>();
    Gramer *gramer = new Gramer();

    /*
    <program> ::= <statement_list>

<statement_list> ::= <statement> | <statement_list> <statement>

<statement> ::= <declaration>
              | <assignment>
              | <conditional>
              | <loop>
              | <outfunc>
              | <expression>

<declaration> ::= TYPE IDENTIFIER SEMICOLOM

<assignment> ::= IDENTIFIER EQUAL <expression> SEMICOLOM

<expression> ::= <term>
               | <expression> OPERATOR <term>

<term> ::= IDENTIFIER
         | LITERAL
         | BRACKET_OPEN <expression> BRACKET_CLOSE

<conditional> ::= IF BRACKET_OPEN <condition> BRACKET_CLOSE CURLY_BRACKET_OPEN <statement_list> CURLY_BRACKET_CLOSE
                | <conditional> ELSE CURLY_BRACKET_OPEN <statement_list> CURLY_BRACKET_CLOSE

<condition> ::= <expression>
              | BOOLEAN

<loop> ::= WHILE BRACKET_OPEN <condition> BRACKET_CLOSE CURLY_BRACKET_OPEN <statement_list> CURLY_BRACKET_CLOSE
          | FOR BRACKET_OPEN <literal> BRACKET_CLOSE CURLY_BRACKET_OPEN <statement_list> CURLY_BRACKET_CLOSE

<outfunc> ::= OUT BRACKET_OPEN <expression> BRACKET_CLOSE SEMICOLOM
    */

    // <statement_list> ::= <statement> | <statement_list> <statement>
    gramer->nonTerminal = STATEMENT_LIST;
    right->push_back(STATEMENT);
    gramer->right = *right;
    parserDFA->AddGrammar(*gramer);
    right->clear();

    right->push_back(STATEMENT_LIST);
    right->push_back(STATEMENT);
    gramer->right = *right;
    parserDFA->AddGrammar(*gramer);
    right->clear();

    // <statement> ::= <declaration> | <assignment> | <conditional> | <loop> | <outfunc> | <expression>
    gramer->nonTerminal = STATEMENT;
    right->push_back(DECLARATION);
    gramer->right = *right;
    parserDFA->AddGrammar(*gramer);
    right->clear();

    right->push_back(ASSIGNMENT);
    gramer->right = *right;
    parserDFA->AddGrammar(*gramer);
    right->clear();

    right->push_back(CONDITIONAL);
    gramer->right = *right;
    parserDFA->AddGrammar(*gramer);
    right->clear();

    right->push_back(LOOP);
    gramer->right = *right;
    parserDFA->AddGrammar(*gramer);
    right->clear();

    right->push_back(OUTFUNC);
    gramer->right = *right;
    parserDFA->AddGrammar(*gramer);
    right->clear();

    right->push_back(EXPRESSION);
    gramer->right = *right;
    parserDFA->AddGrammar(*gramer);
    right->clear();

    // <declaration> ::= TYPE IDENTIFIER SEMICOLOM
    gramer->nonTerminal = DECLARATION;
    right->push_back(TYPE);
    right->push_back(IDENTIFIER);
    right->push_back(SEMICOLON);
    gramer->right = *right;
    parserDFA->AddGrammar(*gramer);
    right->clear();
    
    // <assignment> ::= IDENTIFIER EQUAL <expression> SEMICOLOM
    gramer->nonTerminal = ASSIGNMENT;
    right->push_back(IDENTIFIER);
    right->push_back(EQUAL);
    right->push_back(EXPRESSION);
    right->push_back(SEMICOLON);
    gramer->right = *right;
    parserDFA->AddGrammar(*gramer);
    right->clear();

    // <expression> ::= <term> | <expression> OPERATOR <term>
    gramer->nonTerminal = EXPRESSION;
    right->push_back(TERM);
    gramer->right = *right;
    parserDFA->AddGrammar(*gramer);
    right->clear();

    right->push_back(EXPRESSION);
    right->push_back(OPERATOR);
    right->push_back(TERM);
    gramer->right = *right;
    parserDFA->AddGrammar(*gramer);
    right->clear();

    // <term> ::= IDENTIFIER | LITERAL | BRACKET_OPEN <expression> BRACKET_CLOSE
    gramer->nonTerminal = TERM;
    right->push_back(IDENTIFIER);
    gramer->right = *right;
    parserDFA->AddGrammar(*gramer);
    right->clear();

    right->push_back(LITERAL);
    gramer->right = *right;
    parserDFA->AddGrammar(*gramer);
    right->clear();

    right->push_back(BARCKET_OPEN);
    right->push_back(EXPRESSION);
    right->push_back(BARCKET_CLOSE);
    gramer->right = *right;
    parserDFA->AddGrammar(*gramer);
    right->clear();

    // <conditional> ::= IF BRACKET_OPEN <condition> BRACKET_CLOSE CURLY_BRACKET_OPEN <statement_list> CURLY_BRACKET_CLOSE
    //                | <conditional> ELSE CURLY_BRACKET_OPEN <statement_list> CURLY_BRACKET_CLOSE
    gramer->nonTerminal = CONDITIONAL;
    right->push_back(IF);
    right->push_back(BARCKET_OPEN);
    right->push_back(CONDITION);
    right->push_back(BARCKET_CLOSE);
    right->push_back(CURLY_BRACKET_OPEN);
    right->push_back(STATEMENT_LIST);
    right->push_back(CURLY_BRACKET_CLOSE);
    gramer->right = *right;
    parserDFA->AddGrammar(*gramer);
    right->clear();

    right->push_back(CONDITIONAL);
    right->push_back(ELSE);
    right->push_back(CURLY_BRACKET_OPEN);
    right->push_back(STATEMENT_LIST);
    right->push_back(CURLY_BRACKET_CLOSE);
    gramer->right = *right;
    parserDFA->AddGrammar(*gramer);
    right->clear();

    // <condition> ::= <expression> | BOOLEAN
    gramer->nonTerminal = CONDITION;
    right->push_back(EXPRESSION);
    gramer->right = *right;
    parserDFA->AddGrammar(*gramer);
    right->clear();

    right->push_back(BOOLEAN);
    gramer->right = *right;
    parserDFA->AddGrammar(*gramer);
    right->clear();

    // <loop> ::= WHILE BRACKET_OPEN <condition> BRACKET_CLOSE CURLY_BRACKET_OPEN <statement_list> CURLY_BRACKET_CLOSE
    //          | FOR BRACKET_OPEN <literal> BRACKET_CLOSE CURLY_BRACKET_OPEN <statement_list> CURLY_BRACKET_CLOSE
    gramer->nonTerminal = LOOP;
    right->push_back(WHILE);
    right->push_back(BARCKET_OPEN);
    right->push_back(CONDITION);
    right->push_back(BARCKET_CLOSE);
    right->push_back(CURLY_BRACKET_OPEN);
    right->push_back(STATEMENT_LIST);
    right->push_back(CURLY_BRACKET_CLOSE);
    gramer->right = *right;
    parserDFA->AddGrammar(*gramer);
    right->clear();

    right->push_back(FOR);
    right->push_back(BARCKET_OPEN);
    right->push_back(LITERAL);
    right->push_back(BARCKET_CLOSE);
    right->push_back(CURLY_BRACKET_OPEN);
    right->push_back(STATEMENT_LIST);
    right->push_back(CURLY_BRACKET_CLOSE);
    gramer->right = *right;
    parserDFA->AddGrammar(*gramer);
    right->clear();

    // <outfunc> ::= OUT BRACKET_OPEN <expression> BRACKET_CLOSE SEMICOLOM
    gramer->nonTerminal = OUTFUNC;
    right->push_back(OUT);
    right->push_back(BARCKET_OPEN);
    right->push_back(EXPRESSION);
    right->push_back(BARCKET_CLOSE);
    right->push_back(SEMICOLON);
    gramer->right = *right;
    parserDFA->AddGrammar(*gramer);
    right->clear();

    parserDFA->buildTable(STATEMENT_LIST);
    parserDFA->saveTable("../DFAs/parser/parserDFA.txt", "../DFAs/parser/parserSets.txt", "../DFAs/parser/parserRules.txt");
    parserDFA->savePrettyTable("../DFAs/parser/parserPrettyDFA.txt");
}
