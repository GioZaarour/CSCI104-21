// LetStatement.cpp:
#include "LetStatement.h"

LetStatement::LetStatement(std::string variableName, int value)
	: m_variableName( variableName ), m_value( value )
{}


// The LetStatement version of execute() should make two changes to the
// state of the program:
//
//    * set the value of the appropriate variable
//    * increment the program counter
void LetStatement::execute(ProgramState * state, std::ostream &outf)
{
	//if they made the variable already we just want to change its value rather than add a new one w the same name
	if(state->checkExists(m_variableName))
	{
		state->changeVal(m_variableName, m_value);
	}
	else
	{
		state->newVar(m_variableName, m_value);
	}

	//increment counter
	state->counterPlus();
}
