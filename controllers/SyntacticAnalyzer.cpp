#include "./headers/SyntacticAnalyzer.hpp"

SyntacticAnalyzer::SyntacticAnalyzer(list<Token> tokens, ErrorHandler *error)
{
    this->tokens = tokens;
    this->error = error;
}

AST *SyntacticAnalyzer::parser(string tablePath, string rulesPath)
{
    Token *upNext = new Token();
    upNext->token = "$";
    upNext->type = END;
    this->tokens.push_back(*upNext);

    ParserDFA *parserDFA = new ParserDFA();
    parserDFA->loadTable(tablePath, rulesPath);
    int(*parsingTable)[TERMINAL + NONE_TERMINAL] = parserDFA->getParsingTable();
    Gramer *grammarRules = parserDFA->getGrammarRules();

    Stack<AST> *ASTack = new Stack<AST>(MAX_STACK);
    Stack<int> *shiftReduceStack = new Stack<int>(MAX_STACK);

    shiftReduceStack->push(0);

    if (!this->tokens.empty())
    {
        upNext->token = this->tokens.front().token;
        upNext->type = this->tokens.front().type;
        this->tokens.pop_front();
    }

    int state, terminal, action, suffix, i;

    while (upNext != NULL)
    {
        terminal = upNext->type;
        state = shiftReduceStack->peek();
        action = parsingTable[state][terminal] / 100 * 100;
        suffix = parsingTable[state][terminal] - action;

        if (action == SHIFT)
        {
            shiftReduceStack->push(terminal);
            shiftReduceStack->push(suffix);
            AST *ast = new AST(upNext);
            ASTack->push(*ast);
            if (!this->tokens.empty())
            {
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
            Gramer gramer = grammarRules[suffix];
            int gramerSize = gramer.right.size();
            Token *token = new Token();
            token->token = to_string(gramer.nonTerminal);
            token->type = NONE_TERMINAL;
            AST *ast = new AST(token);
            for (i = 0; i < gramerSize; i++)
            {
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
            return ASTack->pop();
        }
        else
        {
            cout << action << " " << state << endl;
            this->error->syntaxError(upNext->token);
            exit(1);
        }
    }
    return 0;
}

SyntacticAnalyzer::~SyntacticAnalyzer()
{
}
