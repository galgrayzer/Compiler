# Compiler
CC = g++

# Files
ERROR = controllers/ErrorHandler.cpp
LEXER = controllers/LexicalAnalyzer.cpp models/lexer/LexerDFA.cpp
PARSER = controllers/SyntacticAnalyzer.cpp models/parser/ParserDFA.cpp models/AST.cpp

# Output
OUT = compiler

# Build
build:
	$(CC) compiler.cpp $(LEXER) $(PARSER) $(ERROR) -o $(OUT)

# Clean
clean:
	rm -f $(OUT) *.o