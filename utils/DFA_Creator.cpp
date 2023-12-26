using namespace std;

#include <iostream>
#include "../models/DFA.cpp"

int main(int argc, char const *argv[])
{
    DFA *dfa = new DFA();
    dfa->addToken("int");
    dfa->addToken("char");
    dfa->addToken("bool");
    dfa->addToken("if");
    dfa->addToken("else");
    dfa->addToken("while");
    dfa->addToken("for");
    dfa->addToken("true");
    dfa->addToken("false");
    dfa->addToken("out");
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
