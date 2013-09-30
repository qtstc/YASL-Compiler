//Tao Qian
//This file contains implementations of the 
//pStackClass

//The class header is found in pstack.h

#include "stdafx.h"
#include "pstack.h"

pStackClass::pStackClass():top(NULL),lastTerminalPopped(tokenClass(EMPTY_T,EMPTY_ST,EMPTY_LEXEME)){}

void pStackClass::push(tokenClass token)
{
	StackCell *tempCell = new StackCell;
	tempCell->next = top;
	top = tempCell;
	tempCell->type = token.type;
	tempCell->subtype = token.subtype;
}

tokenClass pStackClass::pop()
{
	if(top == NULL)
		return tokenClass(EMPTY_T,EMPTY_ST,EMPTY_LEXEME);
	tokenClass token = toTokenClass(top);
	//Check if the token is a terminal, if yes, update the lastTerminalPoped.
	if(token.type != E_T)
	{
		lastTerminalPopped = token;
	}
	StackCell *temp = top;
	top = top->next;
	delete temp;
	return token;
}

tokenClass pStackClass::getTopMostTerminal()
{
	StackCell *p = top;
	while(p != NULL)
	{
		if(p->type != E_T)
			return toTokenClass(p);
		p = p->next;
	}
	return tokenClass(EMPTY_T,EMPTY_ST,EMPTY_LEXEME);//p == NULL
}

tokenClass pStackClass::toTokenClass(StackCell* cell)
{
	return tokenClass(cell->type,cell->subtype,EMPTY_LEXEME);
}

void pStackClass::clear()
{
	while(top != NULL)
	{
		StackCell *temp = top;
		top = top->next;
		delete temp;
	}
}

bool pStackClass::terminalOnTop()
{
	if(top == NULL)
		return false;
	return top->type != E_T;
}