# Compiler
CC = g++

# Files
ERROR = controllers/ErrorHandler.cpp
LEXER = controllers/LexicalAnalyzer.cpp models/lexer/LexerDFA.cpp
PARSER = controllers/SyntacticAnalyzer.cpp models/parser/ParserDFA.cpp models/AST.cpp
SYMANTIC = controllers/SymanticAnalyzer.cpp models/symantic/Symbol.cpp
CODEGEN = controllers/CodeGeneration.cpp

# Output
OUT = compiler

# Build
build:
	$(CC) compiler.cpp $(ERROR) $(LEXER) $(PARSER) $(SYMANTIC) $(CODEGEN) -o $(OUT)

# Clean
clean:
	rm -f $(OUT) *.o