#pragma once

using namespace std;

#include <vector>
#include "./Token.hpp"
#include "../symantic/headers/Symbol.hpp"
#include <iostream>

class AST
{
private:
    // Attributes
    Token *root;
    vector<AST *> *children;
    int size;
    void printHelper(AST *tree, string prefix, bool isLast);

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
    void print();
    void pushChild(AST *child);
    string getSymbolType(int type);
};