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
    this->printHelper(this, "", true);
}

void AST::printHelper(AST *tree, string prefix, bool isLast)
{
    if (tree->getRoot() == NULL)
        return;

    cout << prefix << (isLast ? "--- " : "|-- ") << tree->root->token << endl;
    for (size_t i = 0; i < tree->getChildren()->size(); ++i)
    {
        // Pass the child node directly, not its reference
        printHelper(tree->getChild(i), prefix + (isLast ? "   " : "|  "), i == tree->getChildren()->size() - 1);
    }
}

// void Parser::printAST(ParseTree& root, string prefix, bool isLast) {
//   cout << prefix << (isLast ? "└── " : "├── ") << root.value << endl;

//   if (!root.children.empty()) {
//     for (size_t i = 0; i < root.children.size(); ++i) {
//       // Pass the child node directly, not its reference
//       printAST(root.children[i], prefix + (isLast ? "   " : "│  "), i == root.children.size() - 1);
//     }
//   }
// } 

AST::~AST()
{
}