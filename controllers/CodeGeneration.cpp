#include "./headers/CodeGeneration.hpp"

/**
 * @brief Constructs a CodeGeneration object.
 *
 * This constructor initializes a CodeGeneration object with the given program and symbol table.
 * It also initializes the label count and registers.
 *
 * @param program The Abstract Syntax Tree (AST) representing the program.
 * @param symbolTable The symbol table containing the symbols used in the program.
 */
CodeGeneration::CodeGeneration(AST *program, unordered_set<Symbol, Symbol::HashFunction> symbolTable)
{
    this->program = program;         // set the program
    this->symbolTable = symbolTable; // set the symbol table
    this->labelCount = 0;            // initialize the label count
    registers[0] = {"eax", true};    // initialize the registers
    registers[1] = {"ebx", true};
    registers[2] = {"ecx", true};
    registers[3] = {"edx", true};
    registers[4] = {"esi", true};
    registers[5] = {"edi", true};
}

/**
 * Generates the code for the given filename.
 *
 * @param filename The name of the file to generate the code into.
 */
void CodeGeneration::generate(string filename)
{
    ofstream file;             // output file
    file.open(filename);       // open the file
    generateBase(file);        // generate the base code
    generateIdentifiers(file); // generate the identifiers
    file << "_start:\n";
    codeFromAST(program); // generate code from the AST
    generateCode(file);   // generate the code
    generateEnd(file);    // generate the end code
    file.close();         // close the file
}

/**
 * Executes code generation for the given Abstract Syntax Tree (AST).
 *
 * @param tree The root of the AST.
 */
void CodeGeneration::codeFromAST(AST *tree)
{
    Token *root = tree->getRoot(); // get the root of the AST
    if (root->type == ASSIGNMENT)  // if the root is an assignment
    {
        assignStatement(tree); // generate code for assignment
        return;
    }
    if (root->type == EXPRESSION) // if the root is an expression
    {
        generateExpression(tree); // generate code for expression
        return;
    }
    if (root->type == OUTFUNC) // if the root is an out function
    {
        outFunction(tree); // generate code for out function
        return;
    }
    if (root->type == LOOP) // if the root is a loop
    {
        int loopType = tree->getChild(0)->getRoot()->type; // get the loop type
        if (loopType == FOR)                               // if the loop type is for
        {
            forStatement(tree); // generate code for for loop
            return;
        }
        else if (loopType == WHILE) // if the loop type is while
        {
            whileStatement(tree); // generate code for while loop
            return;
        }
    }
    if (root->type == CONDITIONAL) // if the root is a conditional
    {
        ifStatement(tree); // generate code for if statement
        return;
    }
    for (AST *child : *tree->getChildren()) // generate code for each child
    {
        codeFromAST(child); // generate code from the child
    }
}

/**
 * Generates the base code for the assembly file.
 *
 * This function writes the initial code for the assembly file, including the BITS directive,
 * the section directives, and the global directive.
 *
 * @param file The output file stream to write the code to.
 */
void CodeGeneration::generateBase(ofstream &file)
{
    file << "BITS 32\n\nsection .text\n\nglobal _start\n\nsection .data\n"; // write the initial code
}

/**
 * Generates the assembly code for the end of the program.
 * Writes the end code and includes a print decimal function.
 *
 * @param file The output file stream to write the code to.
 */
void CodeGeneration::generateEnd(ofstream &file)
{
    file << "\n\tmov eax, 1\n\tmov ebx, 0\n\tint 0x80\n\n"; // write the end code
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
    // print decimal function
    file << "printDecimal:\n\txor ecx, ecx\nmone:\n\tinc ecx\n\txor edx, edx\n\tdiv word [ten]\n\tpush edx\n\tcmp eax, 0\n\tjg mone\n\nputChar:\n\tpop edx\n\tadd edx, '0'\n\tmov [tmp], edx\n\tpusha\n\tmov eax, 4\n\tmov ebx, 1\n\tmov ecx, tmp\n\tmov edx, 2\n\tint 0x80\n\tpopa\n\tloop putChar\n\tret\n";
}

/**
 * Generates identifiers in the output file.
 *
 * This function writes the necessary assembly code to generate identifiers in the output file.
 * It iterates over the symbol table and generates the appropriate assembly code for each symbol.
 * The generated assembly code includes the declaration of variables with their respective types.
 *
 * @param file The output file stream to write the assembly code to.
 */
void CodeGeneration::generateIdentifiers(ofstream &file)
{
    file << "\ttmp db 100 dup(0)\n"; // write the temporary variable
    file << "\tten dw 10\n";         // write the divider
    for (Symbol s : symbolTable)     // iterate over the symbol table
    {
        file << "\t" << s.name << (s.type == CHAR ? " db 0\n" : " dd 0\n"); // write the identifier
    }
    file << "\n"; // write a newline
}

/**
 * Generates code by writing each string in the 'code' vector to the specified output stream.
 *
 * @param file The output stream to write the generated code to.
 */
void CodeGeneration::generateCode(ostream &file) // generate the code
{
    for (string c : code) // iterate over the code vector
    {
        file << c; // write the code to the file
    }
}

/**
 * @brief Adds a code string to the code generation.
 *
 * This function appends the given code string to the internal code vector.
 *
 * @param code The code string to be added.
 */
void CodeGeneration::addCode(string code)
{
    this->code.push_back(code); // add the code to the code vector
}

/**
 * Outputs the result of a function call.
 *
 * @param node The AST node representing the function call.
 */
void CodeGeneration::outFunction(AST *node)
{
    Register reg = generateExpression(node->getChild(2)); // generate the expression
    int type = node->getChild(2)->getRoot()->typeCode;    // get the type of the expression
    if (type == CHAR)                                     // if the type is char
    {
        addCode("\tmov [tmp], " + reg.name + "\n"); // move the result to the temporary variable
        addCode("\tpusha\n");
        addCode("\tmov eax, 4\n");
        addCode("\tmov ebx, 1\n");
        addCode("\tmov ecx, tmp\n");
        addCode("\tmov edx, 2\n");
        addCode("\tint 0x80\n");
        addCode("\tpopa\n");
    }
    else if (type == INT) // if the type is int
    {
        addCode("\tmov eax, " + reg.name + "\n"); // move the result to eax
        addCode("\tcall printDecimal\n");         // call the print decimal function
    }
    freeRegister(reg); // free the register
}

/**
 * Executes the code generation for a for statement in the AST.
 *
 * @param forStatement The AST node representing the for statement.
 */
void CodeGeneration::forStatement(AST *forStatement)
{
    Token *count = forStatement->getChild(2)->getRoot(); // get the count token
    int typeCode = count->typeCode;                      // get the type code
    int labelC = this->labelCount++;                     // get the label count
    Register reg = getRegister();                        // get a register
    addCode("\tmov " + reg.name + ", " + count->token + "\n");
    addCode("for_loop_" + to_string(labelC) + ":\n");
    addCode("\tpush " + reg.name + "\n");
    codeFromAST(forStatement->getChild(5)); // generate code for the body
    addCode("\tpop " + reg.name + "\n");
    addCode("\tdec " + reg.name + "\n");
    addCode("\tjnz for_loop_" + to_string(labelC) + "\n");
}

/**
 * Executes the code generation for a while statement in the AST.
 *
 * @param whileStatement The AST node representing the while statement.
 */
void CodeGeneration::whileStatement(AST *whileStatement)
{
    int count = this->labelCount++; // get the label count
    addCode("while_" + to_string(count) + ":\n");
    Register reg = generateExpression(whileStatement->getChild(2)->getChild(0)); // generate the expression
    addCode("\tcmp " + reg.name + ", 0\n");
    addCode("\tjz end_" + to_string(count) + "\n");
    codeFromAST(whileStatement->getChild(5)); // generate code for the body
    addCode("\tjmp while_" + to_string(count) + "\n");
    addCode("end_" + to_string(count) + ":\n");
    freeRegister(reg); // free the register
}

/**
 * Assigns a value to a variable in the generated code.
 *
 * @param assignStatement The AST node representing the assignment statement.
 */
void CodeGeneration::assignStatement(AST *assignStatement)
{
    Token *identifier = assignStatement->getChild(0)->getRoot();     // get the identifier
    Register reg = generateExpression(assignStatement->getChild(2)); // generate the expression
    addCode("\tmov [" + identifier->token + "], " + reg.name + "\n");
    freeRegister(reg); // free the register
}

/**
 * Executes the code generation for an if statement.
 *
 * @param ifState The AST node representing the if statement.
 */
void CodeGeneration::ifStatement(AST *ifState)
{
    if (ifState->getSize() == 7) // regular if
    {
        Register reg = generateExpression(ifState->getChild(2)->getChild(0)); // generate the expression
        addCode("\tcmp " + reg.name + ", 0\n");
        int count = this->labelCount++; // get the label count
        addCode("\tjz end_" + to_string(count) + "\n");
        codeFromAST(ifState->getChild(5)); // generate code for the body
        addCode("end_" + to_string(count) + ":\n");
        freeRegister(reg); // free the register
    }
    else
    {                                                                      // if with else
        AST *cond = ifState->getChild(0);                                  // get the condition
        Register reg = generateExpression(cond->getChild(2)->getChild(0)); // generate the expression
        addCode("\tcmp " + reg.name + ", 0\n");
        int count = this->labelCount++; // get the label count
        addCode("\tjz else_" + to_string(count) + "\n");
        codeFromAST(cond->getChild(5)); // generate code for the if body
        addCode("\tjmp end_" + to_string(count) + "\n");
        addCode("else_" + to_string(count) + ":\n");
        codeFromAST(ifState->getChild(3)); // generate code for the else body
        addCode("end_" + to_string(count) + ":\n");
        freeRegister(reg); // free the register
    }
}

/**
 * Generates code for an expression in the AST.
 *
 * @param expression The AST node representing the expression.
 * @return The register containing the result of the expression.
 */
Register CodeGeneration::generateExpression(AST *expression)
{
    Token *root = expression->getRoot();                   // get the root of the expression
    if (root->type == IDENTIFIER || root->type == LITERAL) // if the root is an identifier or literal
    {
        Register reg = getRegister();
        addCode("\tmov " + reg.name + ", " + (root->type == IDENTIFIER ? "[" + root->token + "]" : root->token) + "\n");
        expression->setReg(reg);
    }
    else if (root->type == EXPRESSION) // if the root is an expression
    {
        for (AST *child : *expression->getChildren()) // generate code for each child
        {
            generateExpression(child);
        }
        if (expression->getSize() == 3) // if the expression has 3 children
        {
            Register lreg = expression->getChild(0)->getReg();     // get the left register
            Register rreg = expression->getChild(2)->getReg();     // get the right register
            string op = expression->getChild(1)->getRoot()->token; // get the operator
            if (op == "+")                                         // if the operator is addition
            {
                addCode("\tadd " + lreg.name + ", " + rreg.name + "\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "-") // if the operator is subtraction
            {
                addCode("\tsub " + lreg.name + ", " + rreg.name + "\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "*") // if the operator is multiplication
            {
                addCode("\timul " + lreg.name + ", " + rreg.name + "\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "/") // if the operator is division
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
            else if (op == "%") // if the operator is modulo
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
            else if (op == "<") // if the operator is less than
            {
                addCode("\tcmp " + lreg.name + ", " + rreg.name + "\n");
                string reg8 = convert32to8(lreg, 1);
                addCode("\tsetl " + reg8 + "\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == ">") // if the operator is greater than
            {
                addCode("\tcmp " + lreg.name + ", " + rreg.name + "\n");
                string reg8 = convert32to8(lreg, 1);
                addCode("\tsetg " + reg8 + "\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "==") // if the operator is equal to
            {
                addCode("\tcmp " + lreg.name + ", " + rreg.name + "\n");
                string reg8 = convert32to8(lreg, 1);
                addCode("\tsetz " + reg8 + "\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "!=") // if the operator is not equal to
            {
                addCode("\tcmp " + lreg.name + ", " + rreg.name + "\n");
                string reg8 = convert32to8(lreg, 1);
                addCode("\tsetz " + reg8 + "\n");
                addCode("\tnot " + reg8 + "\n");
                addCode("\tand " + lreg.name + ", 1\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "<=") // if the operator is less than or equal to
            {
                addCode("\tcmp " + lreg.name + ", " + rreg.name + "\n");
                string reg8 = convert32to8(lreg, 1);
                addCode("\tsetle " + reg8 + "\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == ">=") // if the operator is greater than or equal to
            {
                addCode("\tcmp " + lreg.name + ", " + rreg.name + "\n");
                string reg8 = convert32to8(lreg, 1);
                addCode("\tsetge " + reg8 + "\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "&") // if the operator is bitwise AND
            {
                addCode("\tand " + lreg.name + ", " + rreg.name + "\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "|") // if the operator is bitwise OR
            {
                addCode("\tor " + lreg.name + ", " + rreg.name + "\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "^") // if the operator is bitwise XOR
            {
                addCode("\txor " + lreg.name + ", " + rreg.name + "\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "!") // if the operator is logical NOT
            {
                addCode("\tnot " + lreg.name + "\n");
                expression->setReg(lreg);
            }
            else if (op == "&&") // if the operator is logical AND
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
            else if (op == "||") // if the operator is logical OR
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
        else // if the expression has only one child
        {
            expression->setReg(expression->getChild(0)->getReg());
        }
    }
    else if (root->type == TERM) // if the root is a term
    {
        if (expression->getSize() == 1) // if the expression has only one child
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

/**
 * Converts a 32-bit register to an 8-bit register.
 *
 * @param reg The 32-bit register to convert.
 * @param byte The byte to determine whether to convert to the low or high 8 bits.
 * @return The corresponding 8-bit register.
 */
string CodeGeneration::convert32to8(Register reg, int byte)
{
    string reg8;
    if (byte == 1) // low byte
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
    else // high byte
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

/**
 * Retrieves a register from the available registers.
 * If there are no free registers, the first register is returned.
 *
 * @return The register object.
 */
Register CodeGeneration::getRegister()
{
    for (int i = 0; i < 6; i++) // iterate over the registers
    {
        if (registers[i].isFree) // if the register is free
        {
            registers[i].isFree = false;
            return registers[i];
        }
    }
    return registers[0];
}

/**
 * Frees the specified register by marking it as available.
 *
 * @param reg The register to be freed.
 */
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

/**
 * Pushes the values of the specified registers onto the stack.
 *
 * @param regs An array of strings representing the registers to be pushed.
 * @param n The number of registers in the array.
 */
void CodeGeneration::pushRegs(string *regs, int n)
{
    for (int i = 0; i < n; i++)
    {
        addCode("\tpush " + regs[i] + "\n");
    }
}

/**
 * Pops the values from the stack into the specified registers.
 *
 * @param regs An array of strings representing the registers to pop the values into.
 * @param n The number of registers to pop the values into.
 */
void CodeGeneration::popRegs(string *regs, int n)
{
    for (int i = n - 1; i >= 0; i--)
    {
        addCode("\tpop " + regs[i] + "\n");
    }
}

/**
 * Sets the state of a register.
 *
 * This function sets the state of the specified register to either free or occupied.
 *
 * @param reg The register to set the state for.
 * @param state The state to set for the register (true for free, false for occupied).
 */
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