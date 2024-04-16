using namespace std;

#include <iostream>
#include "../models/lexer/LexerDFA.cpp"
#include "../models/headers/Token.hpp"

void createLexerDfa()
{
    DFA *dfa = new DFA();
    dfa->addToken("int", TYPE);
    dfa->addToken("char", TYPE);
    dfa->addToken("bool", TYPE);
    dfa->addToken("if", IF);
    dfa->addToken("else", ELSE);
    dfa->addToken("while", WHILE);
    dfa->addToken("for", FOR);
    dfa->addToken("true", BOOLEAN);
    dfa->addToken("false", BOOLEAN);
    dfa->addToken("out", OUT);
    // dfa->addToken("++", OPERATOR);
    // dfa->addToken("--", OPERATOR);
    dfa->addToken("=", EQUAL);
    dfa->addToken("==", OPERATOR);
    dfa->addToken("!=", OPERATOR);
    dfa->addToken("<=", OPERATOR);
    dfa->addToken(">=", OPERATOR);
    dfa->addToken("&&", OPERATOR);
    dfa->addToken("||", OPERATOR);
    dfa->addToken("!", OPERATOR);
    dfa->addToken("+", OPERATOR);
    dfa->addToken("-", OPERATOR);
    dfa->addToken("*", OPERATOR);
    dfa->addToken("/", OPERATOR);
    dfa->addToken("%", OPERATOR);
    dfa->addToken("<", OPERATOR);
    dfa->addToken(">", OPERATOR);
    dfa->addToken("(", BARCKET_OPEN);
    dfa->addToken(")", BARCKET_CLOSE);
    dfa->addToken("{", CURLY_BRACKET_OPEN);
    dfa->addToken("}", CURLY_BRACKET_CLOSE);
    dfa->addToken(";", SEMICOLON);
    dfa->saveTransitionTable("../DFAs/lexer/lexerDFA.txt");
}

int main(int argc, char const *argv[])
{
    createLexerDfa();
    return 0;
}
