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
	tempCell->token = token;
	tempCell->symbol = NULL;
}

void pStackClass::pushE(SymbolNode* symbol)
{
	StackCell *tempCell = new StackCell;
	tempCell->next = top;
	top = tempCell;
	tempCell->token = tokenClass(E_T,NONE_ST,"E");
	tempCell->symbol = symbol;
}

tokenClass pStackClass::pop()
{
	if(top == NULL)
		return tokenClass(EMPTY_T,EMPTY_ST,EMPTY_LEXEME);
	tokenClass token = top->token;
	//Check if the token is a terminal, if yes, update the lastTerminalPoped.
	if(token.type != E_T)
		lastTerminalPopped = token;
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
		if(p->token.type != E_T)
		{
			//cout<<p->token.type<<" "<<E_T<<endl;
			return p->token;
		}
		p = p->next;
	}
	return tokenClass(EMPTY_T,EMPTY_ST,EMPTY_LEXEME);//p == NULL
}

pStackClass::~pStackClass()
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
	return top->token.type != E_T;
}