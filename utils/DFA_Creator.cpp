using namespace std;

#include <iostream>
#include "./models/DFA.cpp"

int main(int argc, char const *argv[])
{
    DFA *dfa = new DFA();
    string token;
    while (true)
    {
        cout << "Enter token: ";
        cin >> token;
        if (token == "-1")
        {
            break;
        }
        dfa->addToken(token);
    }
    dfa->saveToFile("dfa.txt");
    
    return 0;
}
