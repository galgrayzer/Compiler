#include "./headers/SyntacticAnalyzer.hpp"

SyntacticAnalyzer::SyntacticAnalyzer(list<Token> tokens, ErrorHandler *error)
{
    this->tokens = tokens;
    this->error = error;
}

/**
 * @brief Parses the input tokens according to the provided parsing table and grammar rules.
 *
 * @param tablePath Path to the parsing table.
 * @param rulesPath Path to the grammar rules.
 * @return Pointer to the Abstract Syntax Tree (AST) representing the parsed program.
 */
AST *SyntacticAnalyzer::parser(string tablePath, string rulesPath)
{
    // Initialize the lookahead token as END token
    Token *upNext = new Token();
    upNext->token = "$";
    upNext->type = END;
    this->tokens.push_back(*upNext); // Push the END token into the token list

    // Initialize ParserDFA and load parsing table and grammar rules
    ParserDFA *parserDFA = new ParserDFA();
    parserDFA->loadTable(tablePath, rulesPath);
    int(*parsingTable)[TERMINAL + NONE_TERMINAL] = parserDFA->getParsingTable();
    Gramer *grammarRules = parserDFA->getGrammarRules();

    // Initialize stacks for AST and shift-reduce parsing
    Stack<AST> *ASTack = new Stack<AST>(MAX_STACK);
    Stack<int> *shiftReduceStack = new Stack<int>(MAX_STACK);

    shiftReduceStack->push(0); // Push initial state to shift-reduce stack

    // Fetch the first token
    if (!this->tokens.empty())
    {
        upNext->token = this->tokens.front().token;
        upNext->type = this->tokens.front().type;
        this->tokens.pop_front();
    }

    int state, terminal, action, suffix, i;

    // Parsing loop
    while (upNext != NULL)
    {
        terminal = upNext->type;
        state = shiftReduceStack->peek();
        action = parsingTable[state][terminal] / 100 * 100;
        suffix = parsingTable[state][terminal] - action;

        if (action == SHIFT)
        {
            // Shift action
            shiftReduceStack->push(terminal);
            shiftReduceStack->push(suffix);
            AST *ast = new AST(upNext);
            ASTack->push(*ast);
            if (!this->tokens.empty())
            {
                // Get the next token
                upNext->token = this->tokens.front().token;
                upNext->type = this->tokens.front().type;
                this->tokens.pop_front();
            }
            else
            {
                upNext = NULL;
            }
        }
        else if (action == REDUCE)
        {
            // Reduce action
            Gramer gramer = grammarRules[suffix];
            int gramerSize = gramer.right.size();
            Token *token = new Token();
            token->token = parserDFA->ENG_STRINGS[gramer.nonTerminal - TERMINAL];
            token->type = gramer.nonTerminal;
            AST *ast = new AST(token);
            for (i = 0; i < gramerSize; i++)
            {
                // Pop symbols from the stack and construct AST nodes
                shiftReduceStack->pop();
                shiftReduceStack->pop();
                AST *child = ASTack->pop();
                ast->pushChild(child);
            }
            ASTack->push(*ast);
            state = shiftReduceStack->peek();
            shiftReduceStack->push(gramer.nonTerminal);
            shiftReduceStack->push(parsingTable[state][gramer.nonTerminal]);
        }
        else if (action == ACCEPT)
        {
            // Accept action
            Token *token = new Token();
            token->token = "PROGRAM";
            token->type = PROGRAM;
            AST *ast = new AST(token);
            AST *child = ASTack->pop();
            ast->pushChild(child);
            return ast;
        }
        else
        {
            // Error handling for invalid action
            cout << action << " " << state << endl;
            this->error->syntaxError(upNext->token);
            exit(1);
        }
    }
    return 0; // Return NULL if parsing is unsuccessful
}

SyntacticAnalyzer::~SyntacticAnalyzer()
{
}
