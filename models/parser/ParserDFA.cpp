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
    if (right->front() > TERMINAL)
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

list<Gramer> *ParserDFA::closure(list<Gramer> *i, int handled_prev[NONE_TERMINAL])
{
    list<Gramer> *closure = new list<Gramer>(*i);
    set<Gramer> *temp = new set<Gramer>();
    list<Gramer>::iterator it, it2;
    int handled[NONE_TERMINAL] = {0};

    for (it = i->begin(); it != i->end(); it++)
    {
        if (it->right.front() >= TERMINAL)
        {
            for (it2 = this->grammarRules->begin(); it2 != this->grammarRules->end(); it2++)
            {
                if (it2->nonTerminal == it->right.front() && (!handled_prev || handled_prev[it2->nonTerminal - TERMINAL] == 0))
                {
                    handled[it2->nonTerminal - TERMINAL] = 1;
                    temp->insert(*it2);
                }
            }

            closure->splice(closure->end(), *(this->closure(new list(temp->begin(), temp->end()), handled)));
            temp->clear();
        }
    }
    return closure;
}

void ParserDFA::buildTable(int final)
{
    list<Gramer> *i = new list<Gramer>();
    Gramer *gramer = new Gramer();
    gramer->nonTerminal = PROGRAM;
    gramer->right.push_back(final);
    i->push_back(*gramer);
    i = this->closure(i);
    // print
    for (int j = 0; j < i->size(); j++)
    {
        cout << i->front().nonTerminal << " -> ";
        for (list<int>::iterator it = i->front().right.begin(); it != i->front().right.end(); it++)
        {
            cout << *it << " ";
        }
        cout << endl;
        i->pop_front();
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
