//Tao Qian
//This file contains a class definition for pStackClass

//The definitions of these routines can be found in pstack.cpp


#ifndef _pstack
#define _pstack

#include "scanner.h"

//A data structure that serves as a wrapper for tokenClass.
//It has an additional pointer allow us to link the tokens together.
class StackCell
{
public:
	tokenClass token;
	StackCell *next;
};

//A stack data structure that also provides some methods
//that are used in the shift-reduce parsing algorithm.
class pStackClass
{
public:
	pStackClass();
	~pStackClass();
	void push(tokenClass theToken);//Push a token onto the stack.
	//Pop a token.
	//If there is no token,
	//return one that has the type EMPTY_T.
	tokenClass pop();
	//Get the top most terminal in the stack. 
	//If the stack does not contain a terminal,
	//A token with EMPTY_T as type will be returned.
	tokenClass getTopMostTerminal();
	//Keep track of the last terminal that was popped.
	//If no terminal is popped, it is a token with EMPTY_T as type.
	tokenClass lastTerminalPopped;
	//Return true if the token on the top of the stack is a terminal.
	bool terminalOnTop();

private:
	StackCell *top;
};

#endif