using namespace std;

#include <string>

enum SymbolType
{
    INT,
    CHAR,
    BOOL,
};

class Symbol
{
public:
    string name;
    int type;
    int scope;

    Symbol(string name, int type, int scope = 0);

    bool operator==(const Symbol &other) const;

    struct HashFunction
    {
        size_t operator()(const Symbol &symbol) const
        {
            return hash<string>()(symbol.name);
        }
    };
};
