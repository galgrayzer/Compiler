#pragma once

#include <list>

struct Gramer
{
    int nonTerminal;
    std::list<int> right;
};