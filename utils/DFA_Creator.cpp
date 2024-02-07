using namespace std;

#include <iostream>
#include "../models/lexer/LexerDFA.cpp"

void createLexerDfa()
{
    DFA *dfa = new DFA();
    dfa->init();
    dfa->addToken("++");
    dfa->addToken("--");
    dfa->addToken("==");
    dfa->addToken("!=");
    dfa->addToken("<=");
    dfa->addToken(">=");
    dfa->addToken("&&");
    dfa->addToken("||");
    dfa->addToken("!");
    dfa->addToken("+");
    dfa->addToken("-");
    dfa->addToken("*");
    dfa->addToken("/");
    dfa->addToken("%");
    dfa->addToken("=");
    dfa->addToken("<");
    dfa->addToken(">");
    dfa->addToken("(");
    dfa->addToken(")");
    dfa->addToken("{");
    dfa->addToken("}");
    dfa->addToken(";");
    dfa->saveTransitionTable("../DFAs/lexer.txt");
}

int main(int argc, char const *argv[])
{
    createLexerDfa();
    return 0;
}
