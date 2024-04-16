#include "./headers/AST.hpp"

AST::AST()
{
    this->root = NULL;
    this->children = new vector<AST *>();
    this->size = 0;
}

AST::AST(Token *root)
{
    Token *newRoot = new Token();
    newRoot->token = root->token;
    newRoot->type = root->type;
    newRoot->line = root->line;
    newRoot->typeCode = root->typeCode;
    this->root = newRoot;
    this->children = new vector<AST *>();
    this->size = 0;
}

AST::AST(Token *root, vector<AST *> *children, int size)
{
    this->root = root;
    this->children = children;
    this->size = size;
}

void AST::setRoot(Token *root)
{
    this->root = root;
}

Token *AST::getRoot()
{
    return this->root;
}

vector<AST *> *AST::getChildren()
{
    return this->children;
}

int AST::getSize()
{
    return this->size;
}

AST *AST::getChild(int index)
{
    return this->children->at(index);
}

void AST::setChildren(vector<AST *> *children)
{
    this->children = children;
}

void AST::setChild(AST *child, int index)
{
    this->children->at(index) = child;
}

void AST::pushChild(AST *child)
{
    AST *newChild = new AST(child->getRoot(), child->getChildren(), child->getSize());
    this->children->insert(this->children->begin(), newChild);
    this->size++;
}

void AST::print()
{
    this->printHelper(this, "", true);
}

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