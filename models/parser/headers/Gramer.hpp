#pragma once

#include <list>

struct Gramer
{
    int nonTerminal; // non terminal
    std::list<int> right; // right side of the production
};