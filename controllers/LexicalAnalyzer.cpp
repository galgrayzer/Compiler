using namespace std;

#include <fstream>
#include <list>
#include <regex>

#include "../models/Token.cpp"

class LexicalAnalyzer
{
private:
    // Attributes
    char *filePath;

    // Methods
    Token *tokenizer(string token);

public:
    // Constructor and Destructor
    LexicalAnalyzer(char *path);
    ~LexicalAnalyzer();

    // Methods
    list<Token> lexer();
};

LexicalAnalyzer::LexicalAnalyzer(char *path)
{
    this->filePath = path;
}

list<Token> LexicalAnalyzer::lexer()
{
    list<Token> tokens;
    ifstream file(this->filePath);

    string word;
    string token;

    int i = 0;
    while (file >> word)
    {
        int startIndex = 0;
        for (int i = 0; i < word.length(); i++)
        {
            if (word[i] == '*' || word[i] == '/' || word[i] == '%' || word[i] == '(' || word[i] == ')' || word[i] == '{' || word[i] == '}' || word[i] == ';')
            {
                token = word.substr(startIndex, i - startIndex);
                if (token != "")
                    tokens.push_back(*tokenizer(token));
                tokens.push_back(*tokenizer(string(1, word[i])));
                startIndex = i + 1;
            }
            else if (word[i] == '+' || word[i] == '-' || word[i] == '!' || word[i] == '=' || word[i] == '<' || word[i] == '>' || word[i] == '&' || word[i] == '|')
            {
                token = word.substr(startIndex, i - startIndex);
                if (token != "")
                    tokens.push_back(*tokenizer(token));

                if (word[i + 1] == '=' || word[i + 1] == '&' || word[i + 1] == '|')
                {
                    tokens.push_back(*tokenizer(string(1, word[i]) + string(1, word[i + 1]))); // <=, >=, ==, !=, &&, ||, ++, --
                    startIndex = i + 2;
                    i++;
                }
                else
                {
                    tokens.push_back(*tokenizer(string(1, word[i]))); // <, >, &, |, !
                    startIndex = i + 1;
                }
            }
            else if (i == word.length() - 1)
            {
                token = word.substr(startIndex, i - startIndex + 1);
                tokens.push_back(*tokenizer(token));
            }
        }
    }
    file.close();

    return tokens;
}

Token *LexicalAnalyzer::tokenizer(string token)
{
    Token *t = new Token();
    t->token = token;
    if (token == "=" || token == "+" || token == "-" || token == "*" || token == "/" || token == "%" || token == "==" || token == "!=" || token == "<" || token == ">" || token == "<=" || token == ">=" || token == "&&" || token == "&" || token == "|" || token == "||" || token == "++" || token == "--" || token == "!")
    {
        t->type = "OPERATOR";
    }
    else if (token == "if" || token == "else" || token == "while" || token == "for" || token == "out" || token == "int" || token == "char" || token == "bool")
    {
        t->type = "KEYWORD";
    }
    else if (token == "(" || token == ")" || token == "{" || token == "}" || token == ";")
    {
        t->type = "SEPERATOR";
    }
    else if (regex_match(token, regex("[0-9]+")) || regex_match(token, regex("'[a-zA-Z0-9]'")) || token == "true" || token == "false")
    {
        t->type = "LITERAL";
    }
    else if (regex_match(token, regex("[a-zA-Z]+")))
    {
        t->type = "IDENTIFIER";
    }
    else
    {
        t->type = "UNKNOWN";
    }
    return t;
}

LexicalAnalyzer::~LexicalAnalyzer()
{
}
