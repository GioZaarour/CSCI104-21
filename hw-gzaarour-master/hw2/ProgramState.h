// ProgramState.h
//
// CS 104 / Fall 2021
//
// The ProgramState class encapsulates the state of an executing Facile
// program.  The state of a BASIC program consists of three parts:
//
//    * The program counter, which specifies the line number of the
//      statement that will execute next.
//    * A map, as explained by the problem writeup.
//    * A stack of integers, which is used to hold return lines for GOSUB
//      statements.
//
// We've provided you a start on this class, but you'll need to add methods
// to it as you go along.  In particular, you'll need methods to access and
// modify the state; these methods will be called primarily by the various
// execute() methods in the Statement subclasses, to allow the execution of
// a statement to change the state of a program.  For example, executing a
// GOTO statement will cause the program counter to be changed.

#ifndef PROGRAM_STATE_INCLUDED
#define PROGRAM_STATE_INCLUDED

#include <stack>
#include <map>
#include <iterator>

class ProgramState
{
public:
	ProgramState(int);

	//increment counter
	void counterPlus();
	void counterJump(int);

	//stack functions
	void addStack(int);
	int popStack(); //make sure to check if the stack is empty first, so RETURN gets treated as an END statement

	//map functions
	void newVar(std::string, int);
	void changeVal(std::string, int);
	int getVal(std::string);
	//to check if a var exists in the map, for when we want to see whether to make a new var or change the value of an existing one:
	bool checkExists(std::string);
	void printAll();

private:
	int counter = 1; // specify the line # of the next statement 
	int m_numLines; //number of lines in the whole input file
	std::stack<int> gosubStack; //for the subroutine line numbers
	std::map <std::string, int> vars; //for the variables and their values, by default is ordered by key (string in this case)
	std::map <std::string, int>::iterator it = vars.begin(); //iterator we will use to printAll vars in the map

};

#endif
