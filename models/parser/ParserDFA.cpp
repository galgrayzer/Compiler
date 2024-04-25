#include "./headers/ParserDFA.hpp"

/**
 * Constructor for the ParserDFA class.
 * Initializes the followSet, firstSet, becomeSet, and parserDFA arrays.
 * Sets the grammarRules to an empty list and the initialState to 1.
 */
ParserDFA::ParserDFA()
{
    int i, j;                    // iterators
    for (i = 0; i < STATES; i++) // iterate over the states
    {
        for (j = 0; j < TERMINAL + NONE_TERMINAL; j++) // iterate over the terminals and none terminals

        {
            if (i < NONE_TERMINAL && j < TERMINAL) // if the state is less than the number of none terminals and the terminal is less than the number of terminals

            {
                this->followSet[i][j] = this->firstSet[i][j] = 0; // set the follow and first sets to 0
            }
            if (i < NONE_TERMINAL && j < NONE_TERMINAL) // if the state is less than the number of none terminals and the terminal is less than the number of none terminals
            {
                this->becomeSet[i][j] = 0; // set the become set to 0
            }
            this->parserDFA[i][j] = -1; // set the parserDFA to -1
        }
    }
    this->grammarRules = new list<Gramer>(); // initialize the grammar rules list
    this->initialState = 1;                  // set the initial state to 1
}

/**
 * Adds a grammar rule to the ParserDFA.
 *
 * @param gramer The grammar rule to be added.
 */
void ParserDFA::AddGrammar(Gramer gramer)
{
    this->grammarRules->push_back(gramer); // add the grammar rule to the list
    list<int>::iterator it;                // iterator
    list<int> *right = &gramer.right;      // get the right side of the grammar rule
    if (right->front() < TERMINAL)         // if the first element of the right side is less than the number of terminals
    {
        this->firstSet[gramer.nonTerminal - TERMINAL][right->front()] = 1; // set the first set
    }
    else // if the first element of the right side is not less than the number of terminals
    {
        if (right->size() == 1) // if the size of the right side is 1
        {
            this->becomeSet[right->front() - TERMINAL][gramer.nonTerminal - TERMINAL] = 1; // set the become set
            return;
        }
    }
    for (it = right->begin(); it != prev(right->end()); it++) // iterate over the right side
    {
        if (TERMINAL <= *it && *next(it) < TERMINAL) // if the current element is greater than or equal to the number of terminals and the next element is less than the number of terminals
        {
            this->followSet[*it - TERMINAL][*next(it)] = 1; // set the follow set
        }
    }
}

/**
 * @brief Retrieves the grammar rules from the ParserDFA object.
 *
 * @return A pointer to an array of Grammar objects containing the grammar rules.
 */
Gramer *ParserDFA::getGrammarRules()
{
    Gramer *rules = new Gramer[this->grammarRules->size()]; // create an array of grammar rules
    list<Gramer>::iterator it;
    int i = 0;
    for (it = this->grammarRules->begin(); it != this->grammarRules->end(); it++, i++)
    {
        rules[i] = *it; // add the grammar rule to the array
    }
    return rules;
}

/**
 * Performs the starting phase of the parser DFA construction.
 * This function recursively identifies non-terminals and initializes the parser DFA.
 *
 * @param final The final state of the parser DFA.
 */
void ParserDFA::startingPhase(int final)
{
    list<int> *nonTerminals = new list<int>(); // create a list of non-terminals
    for (int i = 0; i < NONE_TERMINAL; i++)    // iterate over the none terminals
    {
        if (this->becomeSet[i][final - TERMINAL]) // if the non-terminal becomes the final state
        {
            nonTerminals->push_back(i + TERMINAL);      // add the non-terminal to the list
            if (this->parserDFA[0][i + TERMINAL] == -1) // if the parserDFA is not initialized
            {
                this->parserDFA[0][i + TERMINAL] = initialState++; // set the initial state
                for (int j = 0; j < TERMINAL; j++)
                {
                    if (this->firstSet[i][j]) // if the first set is not empty
                    {
                        if (this->parserDFA[0][j] == -1)
                        {
                            this->parserDFA[0][j] = SHIFT + this->initialState++; // set the shift
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < TERMINAL; i++) // iterate over the terminals
    {
        if (this->firstSet[final - TERMINAL][i])
        {
            if (this->parserDFA[0][i] == -1)
            {
                this->parserDFA[0][i] = SHIFT + this->initialState++; // set the shift
            }
        }
    }
    list<int>::iterator it;                                           // iterator
    for (it = nonTerminals->begin(); it != nonTerminals->end(); it++) // iterate over the non-terminals
    {
        startingPhase(*it); // perform the starting phase
    }
}

/**
 * Fills the expansions for a given state and final state.
 *
 * @param state The current state.
 * @param final The final state.
 */
void ParserDFA::fillExpansions(int state, int final) // fill expansions
{
    list<int> *nonTerminals = new list<int>(); // create a list of non-terminals
    for (int i = 0; i < NONE_TERMINAL; i++)
    {
        if (this->becomeSet[i][final - TERMINAL]) // if the non-terminal becomes the final state
        {
            nonTerminals->push_back(i + TERMINAL);          // add the non-terminal to the list
            if (this->parserDFA[state][i + TERMINAL] == -1) // if the parserDFA is not initialized
            {
                this->parserDFA[state][i + TERMINAL] = this->parserDFA[0][i + TERMINAL]; // set the parserDFA

                for (int j = 0; j < TERMINAL; j++) // iterate over the terminals
                {
                    if (this->firstSet[i][j]) // if the first set is not empty
                    {
                        if (this->parserDFA[state][j] == -1)
                        {
                            this->parserDFA[state][j] = this->parserDFA[0][j]; // set the parserDFA
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < TERMINAL; i++) // iterate over the terminals
    {
        if (this->firstSet[final - TERMINAL][i]) // if the first set is not empty
        {
            if (this->parserDFA[state][i] == -1)
            {
                this->parserDFA[state][i] = this->parserDFA[0][i]; // set the parserDFA
            }
        }
    }
    list<int>::iterator it;
    for (it = nonTerminals->begin(); it != nonTerminals->end(); it++) // iterate over the non-terminals
    {
        fillExpansions(state, *it); // fill the expansions
    }
}

/**
 * Builds the parsing table for the parser DFA.
 *
 * This function constructs the parsing table for the parser DFA based on the grammar rules and follow sets.
 * It populates the `parserDFA` array with shift and reduce actions for each state and terminal symbol.
 * If a conflict is encountered during the construction of the parsing table, an error message is printed and the program exits.
 *
 * @param final The index of the final state in the DFA.
 */
void ParserDFA::buildTable(int final)
{
    int i, j, state, rule = 0;
    this->followSet[final - TERMINAL][END] = 1; // set the follow set
    for (i = 0; i < NONE_TERMINAL; i++)         // iterate over the non-terminals
    {
        for (j = 0; j < NONE_TERMINAL; j++) // iterate over the non-terminals
        {
            if (this->becomeSet[i][j]) // if the non-terminal becomes the non-terminal
            {
                for (int k = 0; k < TERMINAL; k++) // iterate over the terminals
                {
                    this->followSet[i][k] = this->followSet[i][k] || this->followSet[j][k]; // set the follow set
                }
            }
        }
    }
    this->parserDFA[0][final] = this->initialState++;      // set the initial state
    this->parserDFA[this->initialState - 1][END] = ACCEPT; // set the accept state
    startingPhase(final);
    list<int> *right;
    list<Gramer>::iterator it;
    list<int>::iterator it2;
    for (it = this->grammarRules->begin(); it != this->grammarRules->end(); it++, rule++) // iterate over the grammar rules
    {
        right = &it->right; // get the right side of the grammar rule
        state = 0;
        for (it2 = right->begin(); it2 != right->end(); it2++) // iterate over the right side
        {
            if (*it2 < TERMINAL) // if the element is less than the number of terminals
            {
                if (this->parserDFA[state][*it2] == -1)
                {
                    this->parserDFA[state][*it2] = SHIFT + this->initialState++; // set the shift
                }
                state = this->parserDFA[state][*it2] - SHIFT; // set the state
            }
            else
            {
                if (this->parserDFA[state][*it2] == -1) // if the parserDFA is not initialized
                {
                    this->parserDFA[state][*it2] = this->initialState++; // set the initial state
                    fillExpansions(state, *it2);                         // fill the expansions
                }
                state = this->parserDFA[state][*it2]; // set the state
            }
        }
        for (i = 0; i < TERMINAL; i++) // iterate over the terminals
        {
            if (this->followSet[it->nonTerminal - TERMINAL][i]) // if the follow set is not empty
            {
                if (this->parserDFA[state][i] == -1)
                {
                    this->parserDFA[state][i] = REDUCE + rule; // set the reduce
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
    string row1 = "TY  ID  ;   =   OP  LI  (   )   IF  {   }   EL  BO  WH  FO  OU  $   PR  SL  ST  DE  AS  EX  TE  CL  CO  LO  OU";
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
