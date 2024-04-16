#include "./headers/LexicalAnalyzer.hpp"

LexicalAnalyzer::LexicalAnalyzer(char *path, ErrorHandler *error)
{
    this->error = error;
    this->filePath = path;
    this->dfa = new DFA();
    this->dfa->loadTransitionTable(DFA_PATH);
}

/**
 * @brief Lexically analyzes the input file and generates a list of tokens.
 *
 * @return List of tokens generated from the input file.
 */
list<Token> LexicalAnalyzer::lexer()
{
    // Initialize an empty list to store tokens
    list<Token> tokens;
    bool charOn = false;

    // Open the input file for reading
    ifstream file(this->filePath);
    string line, token = "";

    // Get the transition table from DFA
    int(*transitionTable)[128] = this->dfa->getTransitionTable();
    int lineCount = 0;

    // Read each line from the input file
    while (getline(file, line))
    {
        lineCount++;
        Token *t;
        int currentState = 0;

        // Process each character in the line
        for (int i = 0; i < line.length(); i++)
        {
            // Check if there is a transition defined for the current state and character
            if (transitionTable[currentState][line[i]] != -1)
            {
                // Traverse the DFA until a final state or an invalid transition is reached
                while (transitionTable[currentState][line[i]] != -1)
                {
                    if (line[i] == '\'')
                        charOn = !charOn;

                    token += line[i];
                    currentState = transitionTable[currentState][line[i]];

                    // If an invalid transition is encountered, report a lexical error
                    if (currentState == -2)
                        this->error->lexicalError(token, lineCount);

                    i++;
                }

                // Backtrack to the last valid state and generate a token
                i--;
                t = tokenizer(token, currentState, lineCount);
                tokens.push_back(*t);

                // Reset current state and token for the next token
                currentState = 0;
                token = "";
            }
        }
    }

    // Close the input file
    file.close();

    // check if there is an unterminated character
    if (charOn)
        this->error->lexicalError("Unterminated character", lineCount);

    // Return the list of tokens
    return tokens;
}

Token *LexicalAnalyzer::tokenizer(string token, int state, int line)
{
    Token *t = new Token();
    t->token = token;
    t->type = this->dfa->getStateArray()[state];
    t->line = line;
    return t;
}

LexicalAnalyzer::~LexicalAnalyzer()
{
    delete this->dfa;
}
