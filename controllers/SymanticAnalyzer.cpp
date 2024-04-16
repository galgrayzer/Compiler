#include "./headers/SymanticAnalyzer.hpp"

SymanticAnalyzer::SymanticAnalyzer(AST *parseTree, ErrorHandler *error)
{
    this->error = error;
    this->parseTree = parseTree;
    this->symbolTable = unordered_set<Symbol, Symbol::HashFunction>();
    this->CreateSymbolTable(parseTree, 0);
}

int SymanticAnalyzer::IdentifyType(Token *token)
{
    string literal = token->token;
    if (literal[0] == '\'')
    {
        if (literal.size() != 3)
        {
            error->SymanticError("Invalid char literal " + literal, token->line);
        }
        return CHAR;
    }
    else if (literal == "true" || literal == "false")
    {
        return BOOL;
    }
    else
    {
        return INT;
    }
}

void SymanticAnalyzer::CreateSymbolTable(AST *tree, int scope)
{
    if (tree->getRoot() == NULL)
    {
        return;
    }
    for (int i = 0; i < tree->getChildren()->size(); i++)
    {
        if (tree->getChild(i)->getRoot()->type == CURLY_BRACKET_OPEN)
            scope++;
        CreateSymbolTable(tree->getChild(i), scope);
    }
    Token *token = tree->getRoot();
    if (token->type == LITERAL)
    {
        symbolTable.insert(Symbol(token->token, IdentifyType(token), scope));
    }
    else if (token->type == DECLARATION)
    {
        string type = tree->getChild(0)->getRoot()->token;
        int typeCode = typeMap[type];
        string identifier = tree->getChild(1)->getRoot()->token;
        symbolTable.insert(Symbol(identifier, typeCode, scope));
    }
}

string SymanticAnalyzer::getSymbolType(int type)
{
    switch (type)
    {
    case INT:
        return "int";
    case BOOL:
        return "bool";
    case CHAR:
        return "char";
    default:
        return "unknown";
    }
}

void SymanticAnalyzer::printSymbolTable()
{
    cout << "Symbol Table" << endl;
    cout << "Identifier\tType\tScope" << endl;
    for (auto symbol : symbolTable)
    {
        cout << symbol.name << "\t\t" << this->getSymbolType(symbol.type) << "\t" << symbol.scope << endl;
    }
}

AST *SymanticAnalyzer::symantic()
{
    if (parseTree->getRoot() == NULL)
    {
        return;
    }
    for (int i = 0; i < parseTree->getChildren()->size(); i++)
    {
        symantic(parseTree->getChild(i));
    }
    Token *token = parseTree->getRoot();
    if (token->type == LITERAL)
    {
        symbolTable.insert(Symbol(token->token, IdentifyType(token), scope));
    }
    else if (token->type == DECLARATION)
    {
        string type = parseTree->getChild(0)->getRoot()->token;
        int typeCode = typeMap[type];
        string identifier = tree->getChild(1)->getRoot()->token;
        symbolTable.insert(Symbol(identifier, typeCode, scope));
    }
}

SymanticAnalyzer::~SymanticAnalyzer()
{
}