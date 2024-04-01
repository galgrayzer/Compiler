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
    this->printHelper(this, 0);
}

void AST::printHelper(AST *tree, int level)
{
    if (tree->getRoot() == NULL)
        return;

    // Print indentation based on level
    for (int i = 0; i < level; i++)
        cout << "    ";

    // Print node value
    cout << tree->getRoot()->token << endl;

    // Print children recursively
    vector<AST *> *children = tree->getChildren();
    for (AST *child : *children)
        printHelper(child, level + 1);
}

AST::~AST()
{
}
