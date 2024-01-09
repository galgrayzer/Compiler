using namespace std;

#include <iostream>
#include "../models/DFA.cpp"

int main(int argc, char const *argv[])
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
    return 0;
}
