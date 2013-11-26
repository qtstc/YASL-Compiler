//Tao Qian
//This file contains implementations of the 
//pStackClass

//The class header is found in pstack.h

#include "stdafx.h"
#include "pstack.h"

tokenSymbolClass::tokenSymbolClass(int type,int subtype,string lexeme):tokenClass(type,subtype,lexeme),symbol(NULL){}
tokenSymbolClass::tokenSymbolClass():tokenClass(),symbol(NULL){}
tokenSymbolClass::tokenSymbolClass(int type,int subtype,string lexeme,SymbolNode* symbol):tokenClass(type,subtype,lexeme),symbol(symbol){}
tokenSymbolClass toTokenSymbolClass(tokenClass token)
{
	return tokenSymbolClass(token.type,token.subtype,token.lexeme);
}

pStackClass::pStackClass():top(NULL),lastTerminalPopped(tokenSymbolClass(EMPTY_T,EMPTY_ST,EMPTY_LEXEME)){}

void pStackClass::push(tokenSymbolClass token)
{
	StackCell *tempCell = new StackCell;
	tempCell->next = top;
	top = tempCell;
	tempCell->token = token;
}


tokenSymbolClass pStackClass::pop()
{
	if(top == NULL)
		return tokenSymbolClass(EMPTY_T,EMPTY_ST,EMPTY_LEXEME);
	tokenSymbolClass token = top->token;
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
	return tokenSymbolClass(EMPTY_T,EMPTY_ST,EMPTY_LEXEME);//p == NULL
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