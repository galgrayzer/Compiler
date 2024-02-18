#pragma once

#include <stdlib.h>

#include "./Token.hpp"

class AST
{
private:
    // Attributes
    Token *root;
    AST **children;
    int size;

public:
    // Constructors and Destructor
    AST(Token *root);
    AST(Token *root, AST **children, int size);
    ~AST();

    // Methods
    void setRoot(Token *root);
    Token *getRoot();
    AST **getChildren();
    AST *getChild(int index);
    int getSize();
    void setChildren(AST **children);
    void setChild(AST *child, int index);
    void pushChild(AST *child);
};