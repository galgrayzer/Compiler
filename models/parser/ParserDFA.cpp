#include "./headers/ParserDFA.hpp"

ParserDFA::ParserDFA()
{
    int i, j;
    for (i = 0; i < STATES; i++)
    {
        for (j = 0; j < TERMINAL + NONE_TERMINAL; j++)
        {
            if (i < NONE_TERMINAL && j < TERMINAL)
            {
                this->followSet[i][j] = this->firstSet[i][j] = 0;
            }
            if (i < NONE_TERMINAL && j < NONE_TERMINAL)
            {
                this->becomeSet[i][j] = 0;
            }
            this->parserDFA[i][j] = -1;
        }
    }
    this->grammarRules = new list<Gramer>();
    this->initialState = 1;
}

void ParserDFA::AddGrammar(Gramer gramer)
{
    this->grammarRules->push_back(gramer);
    list<int>::iterator it;
    list<int> *right = &gramer.right;
    if (right->front() < TERMINAL)
    {
        this->firstSet[gramer.nonTerminal - TERMINAL][right->front()] = 1;
    }
    else
    {
        if (right->size() == 1)
        {
            this->becomeSet[right->front() - TERMINAL][gramer.nonTerminal - TERMINAL] = 1;
            return;
        }
    }
    for (it = right->begin(); it != prev(right->end()); it++)
    {
        if (TERMINAL <= *it && *next(it) < TERMINAL)
        {
            this->followSet[*it - TERMINAL][*next(it)] = 1;
        }
    }
}

Gramer *ParserDFA::getGrammarRules()
{
    Gramer *rules = new Gramer[this->grammarRules->size()];
    list<Gramer>::iterator it;
    int i = 0;
    for (it = this->grammarRules->begin(); it != this->grammarRules->end(); it++, i++)
    {
        rules[i] = *it;
    }
    return rules;
}

void ParserDFA::startingPhase(int final)
{
    list<int> *nonTerminals = new list<int>();
    for (int i = 0; i < NONE_TERMINAL; i++)
    {
        if (this->becomeSet[i][final - TERMINAL])
        {
            nonTerminals->push_back(i + TERMINAL);
            if (this->parserDFA[0][i + TERMINAL] == -1)
            {
                this->parserDFA[0][i + TERMINAL] = initialState++;
                for (int j = 0; j < TERMINAL; j++)
                {
                    if (this->firstSet[i][j])
                    {
                        if (this->parserDFA[0][j] == -1)
                        {
                            this->parserDFA[0][j] = SHIFT + this->initialState++;
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < TERMINAL; i++)
    {
        if (this->firstSet[final - TERMINAL][i])
        {
            if (this->parserDFA[0][i] == -1)
            {
                this->parserDFA[0][i] = SHIFT + this->initialState++;
            }
        }
    }
    list<int>::iterator it;
    for (it = nonTerminals->begin(); it != nonTerminals->end(); it++)
    {
        startingPhase(*it);
    }
}

void ParserDFA::fillExpansions(int state, int final)
{
    list<int> *nonTerminals = new list<int>();
    for (int i = 0; i < NONE_TERMINAL; i++)
    {
        if (this->becomeSet[i][final - TERMINAL])
        {
            nonTerminals->push_back(i + TERMINAL);
            if (this->parserDFA[state][i + TERMINAL] == -1)
            {
                this->parserDFA[state][i + TERMINAL] = this->parserDFA[0][i + TERMINAL];

                for (int j = 0; j < TERMINAL; j++)
                {
                    if (this->firstSet[i][j])
                    {
                        if (this->parserDFA[state][j] == -1)
                        {
                            this->parserDFA[state][j] = this->parserDFA[0][j];
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < TERMINAL; i++)
    {
        if (this->firstSet[final - TERMINAL][i])
        {
            if (this->parserDFA[state][i] == -1)
            {
                this->parserDFA[state][i] = this->parserDFA[0][i];
            }
        }
    }
    list<int>::iterator it;
    for (it = nonTerminals->begin(); it != nonTerminals->end(); it++)
    {
        fillExpansions(state, *it);
    }
}

void ParserDFA::buildTable(int final)
{
    int i, j, state, rule = 0;
    this->followSet[final - TERMINAL][END] = 1;
    for (i = 0; i < NONE_TERMINAL; i++)
    {
        for (j = 0; j < NONE_TERMINAL; j++)
        {
            if (this->becomeSet[i][j])
            {
                for (int k = 0; k < TERMINAL; k++)
                {
                    this->followSet[i][k] = this->followSet[i][k] || this->followSet[j][k];
                }
            }
        }
    }
    this->parserDFA[0][final] = this->initialState++;
    this->parserDFA[this->initialState - 1][END] = ACCEPT;
    startingPhase(final);
    list<int> *right;
    list<Gramer>::iterator it;
    list<int>::iterator it2;
    for (it = this->grammarRules->begin(); it != this->grammarRules->end(); it++, rule++)
    {
        right = &it->right;
        state = 0;
        for (it2 = right->begin(); it2 != right->end(); it2++)
        {
            if (*it2 < TERMINAL)
            {
                if (this->parserDFA[state][*it2] == -1)
                {
                    this->parserDFA[state][*it2] = SHIFT + this->initialState++;
                }
                state = this->parserDFA[state][*it2] - SHIFT;
            }
            else
            {
                if (this->parserDFA[state][*it2] == -1)
                {
                    this->parserDFA[state][*it2] = this->initialState++;
                    fillExpansions(state, *it2);
                }
                state = this->parserDFA[state][*it2];
            }
        }
        for (i = 0; i < TERMINAL; i++)
        {
            if (this->followSet[it->nonTerminal - TERMINAL][i])
            {
                if (this->parserDFA[state][i] == -1)
                {
                    this->parserDFA[state][i] = REDUCE + rule;
                }
                else
                {
                    cout << "Conflict in state " << state << " with terminal " << i << " and non terminal " << it->nonTerminal << endl;
                    cout << "Rule " << rule << " and " << this->parserDFA[state][i] << endl;
                    exit(1);
                }
            }
        }
    }
}

void ParserDFA::saveTable(string tablePath, string setsPath, string rulesPath)
{
    ofstream file;
    file.open(tablePath);
    if (file.is_open())
    {
        for (int i = 0; i < STATES; i++)
        {
            for (int j = 0; j < TERMINAL + NONE_TERMINAL; j++)
            {
                file << this->parserDFA[i][j] << " ";
            }
            file << endl;
        }
        file.close();
    }
    else
    {
        cout << "Error opening file" << endl;
    }

    file.open(setsPath);
    if (file.is_open())
    {
        for (int i = 0; i < NONE_TERMINAL; i++)
        {
            for (int j = 0; j < TERMINAL; j++)
            {
                file << this->firstSet[i][j] << " ";
            }
            file << endl;
        }
        file << endl;
        for (int i = 0; i < NONE_TERMINAL; i++)
        {
            for (int j = 0; j < TERMINAL; j++)
            {
                file << this->followSet[i][j] << " ";
            }
            file << endl;
        }
        file << endl;
        for (int i = 0; i < NONE_TERMINAL; i++)
        {
            for (int j = 0; j < NONE_TERMINAL; j++)
            {
                file << this->becomeSet[i][j] << " ";
            }
            file << endl;
        }
        file.close();
    }
    else
    {
        cout << "Error opening file" << endl;
    }

    file.open(rulesPath);
    if (file.is_open())
    {
        list<Gramer>::iterator it;
        list<int>::iterator it2;
        for (it = this->grammarRules->begin(); it != this->grammarRules->end(); it++)
        {
            file << it->nonTerminal << " -> ";
            for (it2 = it->right.begin(); it2 != it->right.end(); it2++)
            {
                file << *it2 << " ";
            }
            file << endl;
        }
        file.close();
    }
    else
    {
        cout << "Error opening file" << endl;
    }
}

void ParserDFA::loadTable(string tablePath, string rulesPath)
{
    ifstream file;
    file.open(tablePath);
    string line, value;
    int i, j, k;
    if (file.is_open())
    {
        for (i = 0; i < STATES; i++)
        {
            getline(file, line);
            value = "";
            k = 0;
            for (j = 0; j < line.length(); j++)
            {
                if (line[j] == ' ')
                {
                    this->parserDFA[i][k++] = stoi(value);
                    value = "";
                }
                else
                {
                    value += line[j];
                }
            }
        }
        file.close();
    }
    else
    {
        cout << "Error opening file" << endl;
    }

    file.open(rulesPath);
    if (file.is_open())
    {
        while (getline(file, line))
        {
            Gramer gramer;
            list<int> *right = new list<int>();
            value = "";
            bool left = true;
            for (i = 0; i < line.length(); i++)
            {
                if (line[i] == ' ')
                {
                    if (left)
                    {
                        gramer.nonTerminal = stoi(value);
                        left = false;
                    }
                    else
                    {
                        if (value != "->")
                        {
                            right->push_back(stoi(value));
                        }
                    }
                    value = "";
                }
                else
                {
                    value += line[i];
                }
            }
            gramer.right = *right;
            this->grammarRules->push_back(gramer);
        }
        file.close();
    }
    else
    {
        cout << "Error opening file" << endl;
    }
}

void ParserDFA::savePrettyTable(string tablePath)
{
    string row1 = "TY  BO  WH  FO  OU  IF  EL  OP  =   {   }   (   )   ;   ID  LI  $   SL  ST  DE  AS  EX  TE  CL  CO  LO  OU";
    ofstream file;
    file.open(tablePath);
    if (file.is_open())
    {
        file << row1 << endl;
        for (int i = 0; i < STATES; i++)
        {
            for (int j = 0; j < TERMINAL + NONE_TERMINAL; j++)
            {
                if (this->parserDFA[i][j] == -1)
                {
                    file << "    ";
                }
                else
                {
                    int len = 0;
                    int temp = this->parserDFA[i][j] % 100;
                    while (temp)
                    {
                        temp /= 10;
                        len++;
                    }
                    if (this->parserDFA[i][j] < 100)
                    {
                        file << this->parserDFA[i][j];
                        for (int k = 0; k < 4 - len; k++)
                        {
                            file << " ";
                        }
                    }
                    else if (this->parserDFA[i][j] >= 100 && this->parserDFA[i][j] < 200)
                    {
                        file << "S" << this->parserDFA[i][j] - 100;
                        for (int k = 0; k < 3 - len; k++)
                        {
                            file << " ";
                        }
                    }
                    else if (this->parserDFA[i][j] >= 200 && this->parserDFA[i][j] < 300)
                    {
                        file << "R" << this->parserDFA[i][j] - 200;
                        for (int k = 0; k < 3 - len; k++)
                        {
                            file << " ";
                        }
                    }
                    else if (this->parserDFA[i][j] >= 300 && this->parserDFA[i][j] < 400)
                    {
                        file << "A" << this->parserDFA[i][j] - 300;
                        for (int k = 0; k < 2 - len; k++)
                        {
                            file << " ";
                        }
                    }
                }
            }
            file << endl;
        }
        file.close();
    }
    else
    {
        cout << "Error opening file" << endl;
    }
}

ParserDFA::~ParserDFA()
{
}
