#include "./headers/Symbol.hpp"

Symbol::Symbol(string name, int type, int scope)
{
    this->name = name;
    this->type = type;
    this->scope = scope;
}

bool Symbol::operator==(const Symbol &other) const
{
    return this->name == other.name;
}