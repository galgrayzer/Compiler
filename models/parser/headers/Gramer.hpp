#pragma once

#include <list>

class Gramer
{
public:
    int nonTerminal;
    std::list<int> right;
    bool operator==(const Gramer &gramer) const
    {
        if (this->nonTerminal != gramer.nonTerminal)
            return false;
        if (this->right.size() != gramer.right.size())
            return false;
        std::list<int> gramerRight = gramer.right;
        std::list<int> thisRight = this->right;
        for (int i = 0; i < this->right.size(); i++)
        {
            if (thisRight.front() != gramerRight.front())
                return false;
            thisRight.pop_front();
            gramerRight.pop_front();
        }
        return true;
    }

    int operator<(const Gramer &gramer) const
    {
        if (this->nonTerminal < gramer.nonTerminal)
            return true;
        if (this->nonTerminal > gramer.nonTerminal)
            return false;
        std::list<int> gramerRight = gramer.right;
        std::list<int> thisRight = this->right;
        for (int i = 0; i < this->right.size(); i++)
        {
            if (thisRight.front() < gramerRight.front())
                return true;
            if (thisRight.front() > gramerRight.front())
                return false;
            thisRight.pop_front();
            gramerRight.pop_front();
        }
        return false;
    }
};