#include "./headers/AST.hpp"

AST::AST()
{
    this->root = NULL;
    this->children = new vector<AST *>();
    this->size = 0;
}

AST::AST(Token *root)
{
    this->root = root;
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
    this->children->push_back(child);
    this->size++;
}

AST::~AST()
{
}
