using namespace std;

#include <iostream>
#include "../models/lexer/LexerDFA.cpp"
#include "../models/Token.cpp"

void createLexerDfa()
{
    DFA *dfa = new DFA();
    dfa->addToken("int", KEYWORD);
    dfa->addToken("char", KEYWORD);
    dfa->addToken("bool", KEYWORD);
    dfa->addToken("if", KEYWORD);
    dfa->addToken("else", KEYWORD);
    dfa->addToken("while", KEYWORD);
    dfa->addToken("for", KEYWORD);
    dfa->addToken("true", KEYWORD);
    dfa->addToken("false", KEYWORD);
    dfa->addToken("out", KEYWORD);
    dfa->addToken("++", OPERATOR);
    dfa->addToken("--", OPERATOR);
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
    dfa->addToken("=", OPERATOR);
    dfa->addToken("<", OPERATOR);
    dfa->addToken(">", OPERATOR);
    dfa->addToken("(", SEPERATOR);
    dfa->addToken(")", SEPERATOR);
    dfa->addToken("{", SEPERATOR);
    dfa->addToken("}", SEPERATOR);
    dfa->addToken(";", SEPERATOR);
    dfa->saveTransitionTable("../DFAs/lexer.txt");
}

int main(int argc, char const *argv[])
{
    createLexerDfa();
    return 0;
}
