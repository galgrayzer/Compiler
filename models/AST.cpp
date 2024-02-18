#include "./headers/AST.hpp"

AST::AST(Token *root)
{
    this->root = root;
}

AST::AST(Token *root, AST **children, int size)
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

AST **AST::getChildren()
{
    return this->children;
}

int AST::getSize()
{
    return this->size;
}

AST *AST::getChild(int index)
{
    if (index >= this->size)
        return NULL;
    return this->children[index];
}

void AST::setChildren(AST **children)
{
    this->children = children;
}

void AST::setChild(AST *child, int index)
{
    this->children[index] = child;
}

void AST::pushChild(AST *child)
{
    this->size++;
    this->children = (AST **)realloc(this->children, this->size * sizeof(AST *));
    this->children[this->size - 1] = child;
}

AST::~AST()
{
}
