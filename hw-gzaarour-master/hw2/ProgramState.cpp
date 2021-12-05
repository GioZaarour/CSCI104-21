#include "ProgramState.h"
#include <stdlib.h>
#include <iostream>

ProgramState::ProgramState(int numLines) : m_numLines(numLines)
{}

void ProgramState::counterPlus()
{
    counter++;
} 

void ProgramState::counterJump(int line)
{
    counter = line;
}

void ProgramState::addStack (int line)
{
    gosubStack.push(line);
}

int ProgramState::popStack()
{
    //if stack is empty we end the program
    if (gosubStack.empty())
    {
        exit(0);
    }
    gosubStack.pop();
}

void ProgramState::newVar(std::string name, int val)
{
    vars.insert( std::pair<std::string, int> (name, val) );
}

void ProgramState::changeVal(std::string name, int val)
{
    it = vars.find(name);
    it->second = val;
}

int ProgramState::getVal(std::string name)
{
    it = vars.find(name);
    return it->second;
}

bool ProgramState::checkExists(std::string name)
{
    if(vars.find(name) == vars.end())
    {
        return false;
    }
    else 
    {
        return true;
    }
}

void ProgramState::printAll()
{
    it = vars.begin();

    while (it != vars.end())
    {
        std::cout << it->first << " " << it->second << std::endl;

        it++;
    }
}