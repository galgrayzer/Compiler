#include "./headers/Symbol.hpp"

Symbol::Symbol(string name, int type, int scope, int line)
{
    this->name = name;
    this->type = type;
    this->scope = scope;
    this->line = line;
}

bool Symbol::operator==(const Symbol &other) const
{
    return this->name == other.name;
}