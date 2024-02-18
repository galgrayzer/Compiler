# Compiler
CC = g++

# Files
FILES = compiler.cpp controllers/ErrorHandler.cpp controllers/LexicalAnalyzer.cpp controllers/SyntacticAnalyzer.cpp models/AST.cpp models/lexer/LexerDFA.cpp

# Output
OUT = compiler

# Build
build:
	$(CC) $(FILES) -o $(OUT)

# Clean
clean:
	rm -f $(OUT) *.o