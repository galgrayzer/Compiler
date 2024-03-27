#pragma once

using namespace std;

#include <vector>
#include "./Token.hpp"

class AST
{
private:
    // Attributes
    Token *root;
    vector<AST *> *children;
    int size;

public:
    // Constructors and Destructor
    AST();
    AST(Token *root);
    AST(Token *root, vector<AST *> *children, int size);
    ~AST();

    // Methods
    void setRoot(Token *root);
    Token *getRoot();
    vector<AST *> *getChildren();
    AST *getChild(int index);
    int getSize();
    void setChildren(vector<AST *> *children);
    void setChild(AST *child, int index);
    void pushChild(AST *child);
};