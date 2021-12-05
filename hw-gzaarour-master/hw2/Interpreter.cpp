// Interpreter.cpp
#include "Statement.h"
#include "LetStatement.h"
#include <vector>
#include <string>
#include <sstream> 
#include <fstream>
#include <cstdlib>
#include <iostream>


using namespace std;

// parseProgram() takes a filename as a parameter, opens and reads the
// contents of the file, and returns an vector of pointers to Statement.
void parseProgram(istream& inf, vector<Statement *> & program);

// parseLine() takes a line from the input file and returns a Statement
// pointer of the appropriate type.  This will be a handy method to call
// within your parseProgram() method.
Statement * parseLine(string line);

// interpretProgram() reads a program from the given input stream
// and interprets it, writing any output to the given output stream.
// Note:  you are required to implement this function!
void interpretProgram(istream& inf, ostream& outf);


int main()
{
        cout << "Enter BASIC program file name: ";
        string filename;
        getline(cin, filename);
        ifstream infile(filename.c_str());
        if (!infile)
        {
                cout << "Cannot open " << filename << "!" << endl;
                return 1;
        }
        interpretProgram(infile, cout);
}



void parseProgram(istream &inf, vector<Statement *> & program)
{
	//starts with NULL in the 0 index so the statements correspond with line numbers
	//basically we fill index 0 with trash data so the vector is now 1 indexed (for us)
	program.push_back(NULL);
	
	string line;
	while( ! inf.eof() )
	{
		getline(inf, line);
		program.push_back( parseLine( line ) );
	}
}


Statement * parseLine(string line)
{
	Statement * statement;	
	stringstream ss;
	string type;
	string var;
	int val;

	ss << line;
	ss >> type;
	
	if ( type == "LET" )
	{
		ss >> var;
		ss >> val;
		// Note:  Because the project spec states that we can assume the file
		//	  contains a syntactically legal program, we know that
		//	  any line that begins with "LET" will be followed by a space
		//	  and then a variable and then an integer value.
		statement = new LetStatement(var, val);
	}
	else if(type == "END")
	{

	}
	else if (type == ".")
	{
		//do what END does
	}

	// Incomplete;  TODO:  Finish this function!
	//need to write parse instructions for every different statement
	//if print or any other command is using a var name that doesn't exist (hasn't been declared), make it and set it to 0
	//END or period end the execution of the program
	//GOTO should terminate w/ error message "Illegal jump instruction" if outside boundaries. use error handling?
	//DIV should terminate when divide by zero. "Divide by zero exception." use integer division so round down
	//IF jumps to a line depending on a comparison. <, <=, >, >=, = (equal to), or <> (not equal to).
	//GOSUB jumps to a line then RETURN sends it back to where it started. if multiple GOSUBs, RETURN jumps back to the MOST RECENT GOSUB statement
	// should also terminate if it jumps outside boundaries. maybe use GOTO within GOSUB?
	//note: execute is what the statement does to the program state and output stream, and the constructor is variables it may store in it
	//for every statement you construct it first, then you execute it. e.g. see LET statement
	// "." should just call EndStatement


		
	return statement;
}


void interpretProgram(istream& inf, ostream& outf)
{
	vector<Statement *> program;
	parseProgram( inf, program );
	
	//ProgramState initialization
	//since index 0 of the program vector is a null pointer, .size() -1 is the actual number of lines 
	ProgramState* state = new ProgramState(program.size() - 1);

	//execute each line in the program in the correct order of execution. 
	//place inside a loop which breaks when the program is supposed to terminate.
	for(int i = 1; i < program.size(); i++)
	{	
		if(program.at(i) == EndStatement)
		{
			break;
		}
		/* else if(program.at(i) == GotoStatement)
		{
			if (program.at(i)->getLine() < 1 || program.at(i)->getLine() > (program.size() - 1) )
			{
				std::outf << "Illegal jump instruction." << endl;
				exit(0);
			}
			else
			{
				i = ( program.at(i)->getLine() ) - 1; //subract 1 because the for loop will do i++ after
			}
		}
		else if(program.at(i) == GosubStatement)
		{

			if (program.at(i)->getLine() < 1 || program.at(i)->getLine() > (program.size() - 1) )
			{
				std::outf << "Illegal jump instruction." << endl;
				exit(0);
			}
			else
			{
				//push the number we need to return to onto the stack
				state.addStack(i);

				i = ( program.at(i)->getLine() ) - 1; //subract 1 because the for loop will do i++ after
			}
		} 
		else if(program.at(i) == IfStatement)
		{
			if (program.at(i)->returnIf)
			{
				if (program.at(i)->getLine() < 1 || program.at(i)->getLine() > (program.size() - 1) )
				{
					std::outf << "Illegal jump instruction." << endl;
					exit(0);
				}
				else
				{
					i = ( program.at(i)->getLine() ) - 1; //subract 1 because the for loop will do i++ after
				}
			}
		}
		else if(program.at(i) == ReturnStatement)
		{
			i = ( program.at(i)->getLine() ) - 1; //subract 1 because the for loop will do i++ after
			state.popStack();
		}*/
		else
		{
			program.at(i)->execute(state, outf);
		}
	}
}
