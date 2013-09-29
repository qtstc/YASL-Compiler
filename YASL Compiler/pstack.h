//Tao Qian
//This file contains a class definition for pStackClass

//The definitions of these routines can be found in pstack.cpp


#ifndef _pstack
#define _pstack

#include "scanner.h"

class StackCell
{
public:
	int type;
	int subtype;

	StackCell *next;
};

class pStackClass
{
public:
	pStackClass();
	void push(tokenClass theToken);

private:
	StackCell *top;
};














#endif