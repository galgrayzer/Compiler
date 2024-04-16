#include "./headres/LexerDFA.hpp"

/**
 * @brief Default constructor to initialize the DFA with default transition rules.
 */
DFA::DFA()
{
    // Initialize initial state
    this->initialState = 1;

    // Set transition rules for alphabets, digits, and underscores
    for (int i = 0; i < TRANSISTION_TABLE_SIZE; i++)
    {
        if ((isalpha(i) || i == '_'))
        {
            // Transition rules for identifiers
            this->transitionTable[0][i] = TRANSISTION_TABLE_SIZE - 2;
            this->transitionTable[TRANSISTION_TABLE_SIZE - 2][i] = TRANSISTION_TABLE_SIZE - 2;
            this->transitionTable[TRANSISTION_TABLE_SIZE - 3][i] = TRANSISTION_TABLE_SIZE - 3;
            this->transitionTable[TRANSISTION_TABLE_SIZE - 1][i] = -2;
        }
        else if (isdigit(i))
        {
            // Transition rules for numeric literals
            this->transitionTable[0][i] = TRANSISTION_TABLE_SIZE - 1;
            this->transitionTable[TRANSISTION_TABLE_SIZE - 1][i] = TRANSISTION_TABLE_SIZE - 1;
            this->transitionTable[TRANSISTION_TABLE_SIZE - 2][i] = TRANSISTION_TABLE_SIZE - 2;
            this->transitionTable[TRANSISTION_TABLE_SIZE - 3][i] = TRANSISTION_TABLE_SIZE - 3;
        }
        else
        {
            // Default transition rules for other characters
            this->transitionTable[0][i] = -1;
            this->transitionTable[TRANSISTION_TABLE_SIZE - 2][i] = -1;
            this->transitionTable[TRANSISTION_TABLE_SIZE - 1][i] = -1;
            this->transitionTable[TRANSISTION_TABLE_SIZE - 3][i] = (i == '\'' ? TRANSISTION_TABLE_SIZE - 4 : TRANSISTION_TABLE_SIZE - 3);
        }
    }

    // Initialize state array
    for (int i = 1; i < TRANSISTION_TABLE_SIZE - 3; i++)
    {
        for (int j = 0; j < TRANSISTION_TABLE_SIZE; j++)
        {
            this->transitionTable[i][j] = -1;
        }
        this->stateArray[i] = IDENTIFIER;
    }
    // Set transition rule for char type
    this->transitionTable[0]['\''] = TRANSISTION_TABLE_SIZE - 3;

    this->stateArray[TRANSISTION_TABLE_SIZE - 2] = IDENTIFIER;
    this->stateArray[TRANSISTION_TABLE_SIZE - 1] = this->stateArray[TRANSISTION_TABLE_SIZE - 4] = LITERAL;
}

/**
 * @brief Fills a specific row in the transition table for identifier tokens.
 *
 * @param row The row index to be filled.
 */
void DFA::fillRowIdentifier(int row)
{
    for (int i = 0; i < TRANSISTION_TABLE_SIZE; i++)
    {
        if ((isalpha(i) || i == '_' || isdigit(i)) && (this->transitionTable[row][i] == -1))
        {
            this->transitionTable[row][i] = TRANSISTION_TABLE_SIZE - 2;
        }
    }
}

bool DFA::needIdentifierFill(int type)
{
    return type == TYPE || type == IF || type == ELSE || type == BOOLEAN || type == WHILE || type == FOR || type == OUT;
}

/**
 * @brief Adds a token to the DFA and updates transition rules accordingly.
 *
 * @param token The token to be added.
 * @param type The type of the token.
 */
void DFA::addToken(string token, int type)
{
    if (this->transitionTable[0][token[0]] == -1 || this->transitionTable[0][token[0]] == TRANSISTION_TABLE_SIZE - 2) // If the transition is not defined
    {
        this->transitionTable[0][token[0]] = this->initialState; // Define the transition
        if (needIdentifierFill(type))
        {
            this->fillRowIdentifier(initialState);
        }
        this->initialState++;
    }
    int currentState = this->transitionTable[0][token[0]];
    for (int i = 1; i < token.length(); i++)
    {
        if (this->transitionTable[currentState][token[i]] == -1 || this->transitionTable[currentState][token[i]] == TRANSISTION_TABLE_SIZE - 2)
        {
            this->transitionTable[currentState][token[i]] = this->initialState;
            this->initialState++;
            if (needIdentifierFill(type))
            {
                this->fillRowIdentifier(initialState);
            }
        }
        currentState = this->transitionTable[currentState][token[i]];
    }
    this->stateArray[currentState] = type;
}

/**
 * @brief Saves the transition table to a file.
 *
 * @param path The path to the file where the transition table will be saved.
 */
void DFA::saveTransitionTable(string path)
{
    ofstream file;
    file.open(path);
    for (int i = 0; i < TRANSISTION_TABLE_SIZE; i++)
    {
        for (int j = 0; j < TRANSISTION_TABLE_SIZE; j++)
        {
            file << this->transitionTable[i][j] << ",";
        }
        file << endl;
    }
    for (int i = 0; i < TRANSISTION_TABLE_SIZE; i++)
    {
        file << this->stateArray[i] << ",";
    }
    file.close();
}

/**
 * @brief Loads the transition table from a file.
 *
 * @param path The path to the file from where the transition table will be loaded.
 */
void DFA::loadTransitionTable(string path)
{
    ifstream file;
    file.open(path);
    string line;
    int i, k, j = 0;
    string token = "";
    for (i = 0; i < TRANSISTION_TABLE_SIZE; i++)
    {
        getline(file, line);
        k = 0;
        for (j = 0; j < line.length(); j++)
        {
            if (line[j] == ',')
            {
                this->transitionTable[i][k] = stoi(token);
                token = "";
                k++;
            }
            else
            {
                token += line[j];
            }
        }
    }
    getline(file, line);
    j = 0;
    string state = "";
    for (i = 0; i < line.length(); i++)
    {
        if (line[i] == ',')
        {
            this->stateArray[j] = stoi(state);
            state = "";
            j++;
        }
        else
        {
            state += line[i];
        }
    }
    file.close();
}

DFA::~DFA()
{
}
