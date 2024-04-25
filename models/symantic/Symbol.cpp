#include "./headers/Symbol.hpp"

Symbol::Symbol(string name, int type, int scope, int line)
{
    this->name = name; // name of the symbol
    this->type = type; // type of the symbol
    this->scope = scope; // scope of the symbol
    this->line = line; // line number
}

bool Symbol::operator==(const Symbol &other) const
{
    return this->name == other.name; // compare names
} 