#include "./headers/SymanticAnalyzer.hpp"

/**
 * @brief Constructs a `SymanticAnalyzer` object.
 *
 * This constructor initializes a `SymanticAnalyzer` object with the given parse tree and error handler.
 * It also creates an empty symbol table and populates it by calling the `CreateSymbolTable` function.
 *
 * @param parseTree The parse tree representing the program.
 * @param error The error handler object for reporting errors.
 */
SymanticAnalyzer::SymanticAnalyzer(AST *parseTree, ErrorHandler *error)
{
    this->error = error;
    this->parseTree = parseTree;
    this->symbolTable = unordered_set<Symbol, Symbol::HashFunction>();
    this->CreateSymbolTable(parseTree, 0);
}

/**
 * Identifies the type of a given token.
 *
 * @param token The token to identify the type of.
 * @return The identified type of the token.
 */
int SymanticAnalyzer::IdentifyType(Token *token)
{
    string literal = token->token;
    if (literal[0] == '\'')
    {
        // Check if the char literal is valid
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

/**
 * Creates a symbol table for the given parse tree.
 *
 * This function creates a symbol table for the given parse tree by iterating through the tree and its children.
 * It inserts the literals and variables into the symbol table.
 *
 * @param tree The parse tree to create the symbol table for.
 * @param scope The scope of the current node in the parse tree.
 */
void SymanticAnalyzer::CreateSymbolTable(AST *tree, int scope)
{
    if (tree->getRoot() == NULL) // Empty tree
    {
        return;
    }
    // Iterate through the children of the tree
    for (int i = 0; i < tree->getChildren()->size(); i++)
    {
        if (tree->getChild(i)->getRoot()->type == CURLY_BRACKET_OPEN) // Increment scope when entering a new block
            scope++;
        CreateSymbolTable(tree->getChild(i), scope); // Recursively create symbol table for the children
    }
    Token *token = tree->getRoot(); // Get the root token of the tree
    if (token->type == DECLARATION)
    {
        string type = tree->getChild(0)->getRoot()->token;
        int typeCode = typeMap[type];                                           // Get the type code of the variable
        string identifier = tree->getChild(1)->getRoot()->token;                // Get the identifier of the variable
        if (symbolTable.find(Symbol(identifier, 0, 0, 0)) != symbolTable.end()) // Check if the variable is already declared
        {
            error->SymanticError("Variable " + identifier + " already declared", tree->getChild(0)->getRoot()->line);
        }
        symbolTable.insert(Symbol(identifier, typeCode, scope, tree->getChild(1)->getRoot()->line)); // Insert the variable into the symbol table
    }
}

void SymanticAnalyzer::printSymbolTable()
{
    cout << "Symbol Table" << endl;
    cout << "Identifier\tType\tScope" << endl;
    for (auto symbol : symbolTable)
    {
        cout << symbol.name << "\t\t" << symbol.type << "\t" << symbol.scope << endl;
    }
}

/**
 * @brief Returns the symbol table of the symantic analyzer.
 *
 * This function returns the symbol table of the symantic analyzer.
 *
 * @return The symbol table of the symantic analyzer.
 */
unordered_set<Symbol, Symbol::HashFunction> SymanticAnalyzer::getSymbolTable()
{
    return this->symbolTable;
}

/**
 * @brief Represents the Abstract Syntax Tree (AST) for the compiler.
 *
 * The AST is a data structure that represents the parsed program in a tree-like form.
 * It is used by the SymanticAnalyzer class to perform symantic analysis on the program.
 *
 * @return Pointer to the root of the AST.
 */
AST *SymanticAnalyzer::symantic()
{
    symanticHelper(parseTree, 0); // Call the symantic helper function to perform symantic analysis
    return parseTree;
}

/**
 * @brief Helper function for the symantic analysis.
 *
 * This function is a helper function for the symantic analysis.
 * It recursively traverses the parse tree and its children to perform symantic analysis.
 * It identifies the type of the tokens and checks for type errors in the program.
 *
 * @param tree The parse tree to perform symantic analysis on.
 * @param scope The scope of the current node in the parse tree.
 * @return The parse tree after performing symantic analysis.
 */
AST *SymanticAnalyzer::symanticHelper(AST *tree, int scope)
{
    if (tree->getRoot() == NULL) // Empty tree
    {
        return tree;
    }
    for (int i = 0; i < tree->getChildren()->size(); i++) // Iterate through the children of the tree
    {
        if (tree->getChild(i)->getRoot()->type == CURLY_BRACKET_OPEN) // Increment scope when entering a new block
            scope++;
        symanticHelper(tree->getChild(i), scope); // Recursively perform symantic analysis on the children
    }
    Token *token = tree->getRoot();
    if (token->type == IDENTIFIER)
    {
        auto symbol = symbolTable.find(Symbol(token->token, 0, 0, 0));                          // Find the identifier in the symbol table
        if (symbol == symbolTable.end() || symbol->scope > scope || symbol->line > token->line) // Check if the identifier is undeclared
        {
            error->SymanticError("Undeclared variable " + token->token, token->line);
        }
        token->typeCode = symbol->type; // Set the type code of the identifier
    }
    else if (token->type == LITERAL)
    {
        token->typeCode = IdentifyType(token); // Identify the type of the literal
    }
    else if (token->type == TERM || token->type == EXPRESSION)
    {
        if (tree->getSize() == 1 || token->type == EXPRESSION)        // Set the type code of the term or expression
            token->typeCode = tree->getChild(0)->getRoot()->typeCode; // Set the type code of the term or expression
        else
            token->typeCode = tree->getChild(1)->getRoot()->typeCode; // Set the type code of the term or expression
    }
    else if (token->type == ASSIGNMENT)
    {
        if (tree->getChild(0)->getRoot()->typeCode != tree->getChild(2)->getRoot()->typeCode) // Check for type errors in the assignment
        {
            error->SymanticError("Invalid Type Error: Cannot assign " + tree->getSymbolType(tree->getChild(2)->getRoot()->typeCode) + " to identifer " + tree->getChild(0)->getRoot()->token + " (" + tree->getSymbolType(tree->getChild(0)->getRoot()->typeCode) + ")", tree->getChild(0)->getRoot()->line);
        }
    }
    if (token->type == EXPRESSION && tree->getChild(0)->getRoot()->type == EXPRESSION) // Check for type errors in the expression
    {
        if (tree->getChild(0)->getRoot()->typeCode != tree->getChild(2)->getRoot()->typeCode) // Check for type errors in the expression
        {
            error->SymanticError("Invalid Type Error: Cannot perform operation on " + tree->getSymbolType(tree->getChild(0)->getRoot()->typeCode) + " and " + tree->getSymbolType(tree->getChild(2)->getRoot()->typeCode), tree->getChild(1)->getRoot()->line);
        }
        token->typeCode = tree->getChild(0)->getRoot()->typeCode; // Set the type code of the expression
    }
    tree->setRoot(token); // Set the root of the tree
    return tree;
}

SymanticAnalyzer::~SymanticAnalyzer()
{
}