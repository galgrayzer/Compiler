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
        else if (loopType == WHILE)
        {
            whileStatement(tree);
            return;
        }
    }
    if (root->type == CONDITIONAL)
    {
        ifStatement(tree);
        return;
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
    file << "\n\tmov eax, 1\n\tmov ebx, 0\n\tint 0x80\n\n";
    // print decimal function
    /*
    printDecimal:
    xor ecx, ecx ; counter
mone:
    inc ecx ; counter++
    xor edx, edx ; dx = 0
    div word [ten] ; ax = ax / divider, dx = ax % divider
    push edx ; push dx to the stack
    cmp eax, 0 ; if ax == 0
    jg mone ; jump to mone

putChar:
    pop edx ; pop dx from the stack
    add edx, '0' ; convert dx to ASCII
    mov [tmp], edx ; store dx to tmp
    pusha ; push all registers to the stack
    mov eax, 4 ; syscall number
    mov ebx, 1 ; file descriptor
    mov ecx, tmp ; pointer to the string
    mov edx, 2 ; length of the string
    int 0x80 ; call kernel
    popa ; pop all registers from the stack
    loop putChar ; loop putChar

    ret ; return to the caller
    */
    file << "printDecimal:\n\txor ecx, ecx\nmone:\n\tinc ecx\n\txor edx, edx\n\tdiv word [ten]\n\tpush edx\n\tcmp eax, 0\n\tjg mone\n\nputChar:\n\tpop edx\n\tadd edx, '0'\n\tmov [tmp], edx\n\tpusha\n\tmov eax, 4\n\tmov ebx, 1\n\tmov ecx, tmp\n\tmov edx, 2\n\tint 0x80\n\tpopa\n\tloop putChar\n\tret\n";
}

void CodeGeneration::generateIdentifiers(ofstream &file)
{
    file << "\ttmp db 100 dup(0)\n";
    file << "\tten dw 10\n";
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
    {
        addCode("\tmov [tmp], " + reg.name + "\n");
        addCode("\tpusha\n");
        addCode("\tmov eax, 4\n");
        addCode("\tmov ebx, 1\n");
        addCode("\tmov ecx, tmp\n");
        addCode("\tmov edx, 2\n");
        addCode("\tint 0x80\n");
        addCode("\tpopa\n");
    }
    else if (type == INT)
    {
        addCode("\tmov eax, " + reg.name + "\n");
        addCode("\tcall printDecimal\n");
    }
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
    addCode("\tpush " + reg.name + "\n");
    codeFromAST(forStatement->getChild(5));
    addCode("\tpop " + reg.name + "\n");
    addCode("\tdec " + reg.name + "\n");
    addCode("\tjnz for_loop_" + to_string(labelC) + "\n");
}

void CodeGeneration::whileStatement(AST *whileStatement)
{
    int count = this->labelCount++;
    addCode("while_" + to_string(count) + ":\n");
    Register reg = generateExpression(whileStatement->getChild(2)->getChild(0));
    addCode("\tcmp " + reg.name + ", 0\n");
    addCode("\tjz end_" + to_string(count) + "\n");
    codeFromAST(whileStatement->getChild(5));
    addCode("\tjmp while_" + to_string(count) + "\n");
    addCode("end_" + to_string(count) + ":\n");
    freeRegister(reg);
}

void CodeGeneration::assignStatement(AST *assignStatement)
{
    Token *identifier = assignStatement->getChild(0)->getRoot();
    Register reg = generateExpression(assignStatement->getChild(2));
    addCode("\tmov [" + identifier->token + "], " + reg.name + "\n");
    freeRegister(reg);
}

void CodeGeneration::ifStatement(AST *ifState)
{
    if (ifState->getSize() == 7) // regular if
    {
        Register reg = generateExpression(ifState->getChild(2)->getChild(0));
        addCode("\tcmp " + reg.name + ", 0\n");
        int count = this->labelCount++;
        addCode("\tjz end_" + to_string(count) + "\n");
        codeFromAST(ifState->getChild(5));
        addCode("end_" + to_string(count) + ":\n");
        freeRegister(reg);
    }
    else
    { // if with else
        AST *cond = ifState->getChild(0);
        Register reg = generateExpression(cond->getChild(2)->getChild(0));
        addCode("\tcmp " + reg.name + ", 0\n");
        int count = this->labelCount++;
        addCode("\tjz else_" + to_string(count) + "\n");
        codeFromAST(cond->getChild(5));
        addCode("\tjmp end_" + to_string(count) + "\n");
        addCode("else_" + to_string(count) + ":\n");
        codeFromAST(ifState->getChild(3));
        addCode("end_" + to_string(count) + ":\n");
        freeRegister(reg);
    }
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
            else if (op == "/")
            {
                addCode("\tpush edx\n");
                addCode("\tpush eax\n");
                addCode("\tmov eax, " + lreg.name + "\n");
                Register reg = getRegister();
                addCode("\tmov " + reg.name + ", " + rreg.name + "\n");
                addCode("\txor edx, edx\n");
                addCode("\tidiv " + reg.name + "\n");
                addCode("\tmov " + reg.name + ", eax\n");
                addCode("\tpop eax\n");
                addCode("\tpop edx\n");
                addCode("\tmov " + lreg.name + ", " + reg.name + "\n");
                freeRegister(reg);
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "%")
            {
                addCode("\tpush edx\n");
                addCode("\tpush eax\n");
                addCode("\tmov eax, " + lreg.name + "\n");
                setRegState({"edx", true}, false);
                Register reg = getRegister();
                addCode("\tmov " + reg.name + ", " + rreg.name + "\n");
                addCode("\txor edx, edx\n");
                setRegState({"edx", false}, true);
                addCode("\tidiv " + reg.name + "\n");
                addCode("\tmov " + reg.name + ", edx\n");
                addCode("\tpop eax\n");
                addCode("\tpop edx\n");
                addCode("\tmov " + lreg.name + ", " + reg.name + "\n");
                freeRegister(reg);
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "<")
            {
                addCode("\tcmp " + lreg.name + ", " + rreg.name + "\n");
                string reg8 = convert32to8(lreg, 1);
                addCode("\tsetl " + reg8 + "\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == ">")
            {
                addCode("\tcmp " + lreg.name + ", " + rreg.name + "\n");
                string reg8 = convert32to8(lreg, 1);
                addCode("\tsetg " + reg8 + "\n");
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
                addCode("\tand " + lreg.name + ", 1\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "<=")
            {
                addCode("\tcmp " + lreg.name + ", " + rreg.name + "\n");
                string reg8 = convert32to8(lreg, 1);
                addCode("\tsetle " + reg8 + "\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == ">=")
            {
                addCode("\tcmp " + lreg.name + ", " + rreg.name + "\n");
                string reg8 = convert32to8(lreg, 1);
                addCode("\tsetge " + reg8 + "\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "&")
            {
                addCode("\tand " + lreg.name + ", " + rreg.name + "\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "|")
            {
                addCode("\tor " + lreg.name + ", " + rreg.name + "\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "^")
            {
                addCode("\txor " + lreg.name + ", " + rreg.name + "\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "!")
            {
                addCode("\tnot " + lreg.name + "\n");
                expression->setReg(lreg);
            }
            else if (op == "&&")
            {
                int count = this->labelCount++;

                // Evaluate the first expression and jump to falseLabel if it's false
                addCode("\tcmp " + lreg.name + ", 0\n");
                addCode("\tje falseLabel_" + to_string(count) + "\n");

                // Evaluate the second expression
                addCode("\tcmp " + rreg.name + ", 0\n");
                addCode("\tje falseLabel_" + to_string(count) + "\n");

                // If we're here, the first expression was true. The result of the AND operation
                addCode("\tmov " + lreg.name + ", 1\n");
                addCode("\tjmp endLabel_" + to_string(count) + "\n");

                // Label for the case where the first expression is false
                addCode("falseLabel_" + to_string(count) + ":\n");
                addCode("\tmov " + lreg.name + ", 0\n"); // Set result to false

                // End label
                addCode("endLabel_" + to_string(count) + ":\n");

                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "||")
            {
                int count = this->labelCount++;

                // Evaluate the first expression and jump to trueLabel if it's true
                addCode("\tcmp " + lreg.name + ", 0\n");
                addCode("\tjne trueLabel_" + to_string(count) + "\n");

                // Evaluate the second expression
                addCode("\tcmp " + rreg.name + ", 0\n");
                addCode("\tjne trueLabel_" + to_string(count) + "\n");

                // If we're here, the first expression was false. The result of the OR operation
                addCode("\tmov " + lreg.name + ", 0\n");
                addCode("\tjmp endLabel_" + to_string(count) + "\n");

                // Label for the case where the first expression is true
                addCode("trueLabel_" + to_string(count) + ":\n");
                addCode("\tmov " + lreg.name + ", 1\n"); // Set result to true

                // End label
                addCode("endLabel_" + to_string(count) + ":\n");

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