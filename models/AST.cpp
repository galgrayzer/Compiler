#include "./headers/AST.hpp"

/**
 * @brief Constructs an AST object.
 * 
 * This constructor initializes the AST object by setting the root to NULL,
 * creating an empty vector for children of the root, and setting the size of the tree to 0.
 */
AST::AST()
{
    this->root = NULL; // root of the tree
    this->children = new vector<AST *>(); // children of the root
    this->size = 0; // size of the tree
}

/**
 * @brief Constructs an AST object with the given root token.
 * 
 * @param root The root token of the AST.
 */
AST::AST(Token *root)
{
    Token *newRoot = new Token(); // create a new token
    newRoot->token = root->token; // set the token value
    newRoot->type = root->type; // set the token type
    newRoot->line = root->line; // set the line number
    newRoot->typeCode = root->typeCode;
    this->root = newRoot;
    this->children = new vector<AST *>(); // children of the root
    this->size = 0; // size of the tree
}

/**
 * @brief Constructs an AST object.
 * 
 * @param root The root token of the AST.
 * @param children A vector of child AST nodes.
 * @param size The size of the children vector.
 */
AST::AST(Token *root, vector<AST *> *children, int size)
{
    this->root = root; // root of the tree
    this->children = children; // children of the root
    this->size = size; // size of the tree
}

/**
 * @brief Sets the root token of the Abstract Syntax Tree (AST).
 * 
 * This function is used to set the root token of the AST. The root token represents the top-level node
 * of the AST, from which all other nodes can be traversed.
 * 
 * @param root A pointer to the root token.
 */
void AST::setRoot(Token *root)
{
    this->root = root;
}

/**
 * Sets the register value for the AST node.
 * 
 * @param reg The register value to set.
 */
void AST::setReg(Register reg)
{
    this->reg = reg;
}

/**
 * Returns the register associated with the AST node.
 *
 * @return The register associated with the AST node.
 */
Register AST::getReg()
{
    return this->reg;
}

/**
 * @brief Get the root token of the Abstract Syntax Tree (AST).
 * 
 * @return Token* The root token of the AST.
 */
Token *AST::getRoot()
{
    return this->root;
}

/**
 * Returns a pointer to the vector of children nodes.
 *
 * @return A pointer to the vector of children nodes.
 */
vector<AST *> *AST::getChildren()
{
    return this->children;
}

/**
 * @brief Gets the size of the AST.
 * 
 * @return The size of the AST.
 */
int AST::getSize()
{
    return this->size;
}

/**
 * Returns the child AST node at the specified index.
 *
 * @param index The index of the child node to retrieve.
 * @return The child AST node at the specified index.
 */
AST *AST::getChild(int index)
{
    return this->children->at(index);
}

/**
 * @brief Sets the children of the AST node.
 * 
 * This function sets the children of the AST node to the given vector of AST pointers.
 * 
 * @param children A pointer to a vector of AST pointers representing the children of the AST node.
 */
void AST::setChildren(vector<AST *> *children)
{
    this->children = children;
}

/**
 * Sets the child AST node at the specified index.
 * 
 * @param child The AST node to set as a child.
 * @param index The index at which to set the child node.
 */
void AST::setChild(AST *child, int index)
{
    this->children->at(index) = child;
}

/**
 * @brief Adds a child AST node to the current AST node.
 * 
 * @param child The child AST node to be added.
 */
void AST::pushChild(AST *child)
{
    AST *newChild = new AST(child->getRoot(), child->getChildren(), child->getSize()); // create a new child
    this->children->insert(this->children->begin(), newChild); // insert the child at the beginning
    this->size++;
}

/**
 * Prints the AST (Abstract Syntax Tree).
 */
void AST::print()
{
    this->printHelper(this, "", true);
}

/**
 * Returns the string representation of the symbol type based on the given type.
 *
 * @param type The type of the symbol.
 * @return The string representation of the symbol type.
 */
string AST::getSymbolType(int type)
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

/**
 * Recursively prints the nodes of the AST tree.
 *
 * @param tree The AST tree to be printed.
 * @param prefix The prefix string used for indentation.
 * @param isLast A boolean value indicating if the current node is the last child of its parent.
 */
void AST::printHelper(AST *tree, string prefix, bool isLast)
{
    if (tree->getRoot() == NULL)
        return;

    // Print the root node                                                                                TERM                     EXPRESSION
    if (tree->getRoot()->type == LITERAL || tree->getRoot()->type == IDENTIFIER || tree->getRoot()->type == 22 || tree->getRoot()->type == 23)
        cout << prefix << (isLast ? "--- " : "|-- ") << tree->root->token << " (" << this->getSymbolType(tree->root->typeCode) << ")" << endl;
    else
        cout << prefix << (isLast ? "--- " : "|-- ") << tree->root->token << endl;

    // Print the children nodes
    for (size_t i = 0; i < tree->getChildren()->size(); ++i)
    {
        // Pass the child node directly, not its reference
        printHelper(tree->getChild(i), prefix + (isLast ? "   " : "|  "), i == tree->getChildren()->size() - 1);
    }
}

AST::~AST()
{
}