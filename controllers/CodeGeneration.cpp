#include "./headers/CodeGeneration.hpp"

CodeGeneration::CodeGeneration(AST *program, unordered_set<Symbol, Symbol::HashFunction> symbolTable)
{
    this->program = program;
    this->symbolTable = symbolTable;
    this->labelCount = 0;
    registers[0] = {"eax", true};
    registers[1] = {"ebx", true};
    registers[2] = {"ecx", true};
    registers[3] = {"edx", true};
    registers[4] = {"esi", true};
    registers[5] = {"edi", true};
}

void CodeGeneration::generate(string filename)
{
    ofstream file;
    file.open(filename);
    generateBase(file);
    generateIdentifiers(file);
    file << "_start:\n";
    codeFromAST(program);
    generateCode(file);
    generateEnd(file);
    file.close();
}

void CodeGeneration::codeFromAST(AST *tree)
{
    Token *root = tree->getRoot();
    if (root->type == ASSIGNMENT)
    {
        assignStatement(tree);
        return;
    }
    if (root->type == EXPRESSION)
    {
        generateExpression(tree);
        return;
    }
    if (root->type == OUTFUNC)
    {
        outFunction(tree);
        return;
    }
    if (root->type == LOOP)
    {
        int loopType = tree->getChild(0)->getRoot()->type;
        if (loopType == FOR)
        {
            forStatement(tree);
            return;
        }
    }
    for (AST *child : *tree->getChildren())
    {
        codeFromAST(child);
    }
}

void CodeGeneration::generateBase(ofstream &file)
{
    file << "BITS 32\n\nsection .text\n\nglobal _start\n\nsection .data\n";
}

void CodeGeneration::generateEnd(ofstream &file)
{
    file << "\tmov eax, 1\n\tmov ebx, 0\n\tint 0x80\n";
}

void CodeGeneration::generateIdentifiers(ofstream &file)
{
    file << "\ttmp db 100 dup(0)\n";
    for (Symbol s : symbolTable)
    {
        file << "\t" << s.name << (s.type == CHAR ? " db 0\n" : " dd 0\n");
    }
    file << "\n";
}

void CodeGeneration::generateCode(ostream &file)
{
    for (string c : code)
    {
        file << c;
    }
}

void CodeGeneration::addCode(string code)
{
    this->code.push_back(code);
}

void CodeGeneration::outFunction(AST *node)
{
    Register reg = generateExpression(node->getChild(2));
    int type = node->getChild(2)->getRoot()->typeCode;
    if (type == CHAR)
        addCode("\tmov [tmp], " + reg.name + "\n");
    else if (type == INT)
        addCode("\tmov [tmp], " + reg.name + "\n");
    addCode("\tpusha\n");
    addCode("\tmov eax, 4\n");
    addCode("\tmov ebx, 1\n");
    addCode("\tmov ecx, tmp\n");
    addCode("\tmov edx, 2\n");
    addCode("\tint 0x80\n");
    addCode("\tpopa\n");
    freeRegister(reg);
}

void CodeGeneration::forStatement(AST *forStatement)
{
    Token *count = forStatement->getChild(2)->getRoot();
    int typeCode = count->typeCode;
    int labelC = this->labelCount++;
    Register reg = getRegister();
    addCode("\tmov " + reg.name + ", " + count->token + "\n");
    addCode("for_loop_" + to_string(labelC) + ":\n");
    codeFromAST(forStatement->getChild(5));
    addCode("\tdec " + reg.name + "\n");
    addCode("\tjnz for_loop_" + to_string(labelC) + "\n");
}

void CodeGeneration::assignStatement(AST *assignStatement)
{
    Token *identifier = assignStatement->getChild(0)->getRoot();
    Register reg = generateExpression(assignStatement->getChild(2));
    addCode("\tmov [" + identifier->token + "], " + reg.name + "\n");
    freeRegister(reg);
}

Register CodeGeneration::generateExpression(AST *expression)
{
    Token *root = expression->getRoot();
    if (root->type == IDENTIFIER || root->type == LITERAL)
    {
        Register reg = getRegister();
        addCode("\tmov " + reg.name + ", " + (root->type == IDENTIFIER ? "[" + root->token + "]" : root->token) + "\n");
        expression->setReg(reg);
    }
    else if (root->type == EXPRESSION)
    {
        for (AST *child : *expression->getChildren())
        {
            generateExpression(child);
        }
        if (expression->getSize() == 3)
        {
            Register lreg = expression->getChild(0)->getReg();
            Register rreg = expression->getChild(2)->getReg();
            string op = expression->getChild(1)->getRoot()->token;
            if (op == "+")
            {
                addCode("\tadd " + lreg.name + ", " + rreg.name + "\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "-")
            {
                addCode("\tsub " + lreg.name + ", " + rreg.name + "\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "*")
            {
                addCode("\timul " + lreg.name + ", " + rreg.name + "\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "==")
            {
                addCode("\tcmp " + lreg.name + ", " + rreg.name + "\n");
                string reg8 = convert32to8(lreg, 1);
                addCode("\tsetz " + reg8 + "\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "!=")
            {
                addCode("\tcmp " + lreg.name + ", " + rreg.name + "\n");
                string reg8 = convert32to8(lreg, 1);
                addCode("\tsetz " + reg8 + "\n");
                addCode("\tnot " + reg8 + "\n");
                addCode("\tadd " + reg8 + ", " + "2" + "\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
        }
        else
        {
            expression->setReg(expression->getChild(0)->getReg());
        }
    }
    else if (root->type == TERM)
    {
        if (expression->getSize() == 1)
        {
            Register reg = generateExpression(expression->getChild(0));
            expression->setReg(reg);
        }
        else
        {
            Register reg = generateExpression(expression->getChild(1));
            expression->setReg(reg);
        }
    }
    return expression->getReg();
}

string CodeGeneration::convert32to8(Register reg, int byte)
{
    string reg8;
    if (byte == 1)
    {
        if (reg.name == "eax")
            reg8 = "al";
        else if (reg.name == "ebx")
            reg8 = "bl";
        else if (reg.name == "ecx")
            reg8 = "cl";
        else if (reg.name == "edx")
            reg8 = "dl";
    }
    else
    {
        if (reg.name == "eax")
            reg8 = "ah";
        else if (reg.name == "ebx")
            reg8 = "bh";
        else if (reg.name == "ecx")
            reg8 = "ch";
        else if (reg.name == "edx")
            reg8 = "dh";
    }
    return reg8;
}

Register CodeGeneration::getRegister()
{
    for (int i = 0; i < 6; i++)
    {
        if (registers[i].isFree)
        {
            registers[i].isFree = false;
            return registers[i];
        }
    }
    return registers[0];
}

void CodeGeneration::freeRegister(Register reg)
{
    for (int i = 0; i < 6; i++)
    {
        if (registers[i].name == reg.name)
        {
            registers[i].isFree = true;
            break;
        }
    }
}

void CodeGeneration::pushRegs(string *regs, int n)
{
    for (int i = 0; i < n; i++)
    {
        addCode("\tpush " + regs[i] + "\n");
    }
}

void CodeGeneration::popRegs(string *regs, int n)
{
    for (int i = n - 1; i >= 0; i--)
    {
        addCode("\tpop " + regs[i] + "\n");
    }
}

void CodeGeneration::setRegState(Register reg, bool state)
{
    for (int i = 0; i < 6; i++)
    {
        if (registers[i].name == reg.name)
        {
            registers[i].isFree = state;
            break;
        }
    }
}

CodeGeneration::~CodeGeneration()
{
}