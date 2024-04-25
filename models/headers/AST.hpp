#pragma once

using namespace std;

#include <vector>
#include "./Token.hpp"
#include "../symantic/headers/Symbol.hpp"
#include "../codegen/headers/Register.hpp"
#include <iostream>

class AST
{
private:
    // Attributes
    Token *root;                                             // root of the tree
    vector<AST *> *children;                                 // children of the root
    int size;                                                // size of the tree
    Register reg;                                            // register for the node
    void printHelper(AST *tree, string prefix, bool isLast); // helper function for printing

public:
    // Constructors and Destructor
    AST();
    AST(Token *root);
    AST(Token *root, vector<AST *> *children, int size);
    ~AST();

    // Methods
    void setRoot(Token *root);                 // set root of the tree
    void setReg(Register reg);                 // set register of the node
    Register getReg();                         // get register of the node
    Token *getRoot();                          // get root of the tree
    vector<AST *> *getChildren();              // get children of the root
    AST *getChild(int index);                  // get child at index
    int getSize();                             // get size of the tree
    void setChildren(vector<AST *> *children); // set children of the root
    void setChild(AST *child, int index);      // set child at index
    void print();                              // print the tree
    void pushChild(AST *child);                // push child to the children
    string getSymbolType(int type);            // get type of the symbol
};