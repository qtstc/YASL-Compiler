//Tao Qian
//This file contains implementations of the 
//pStackClass

//The class header is found in pstack.h

#include "stdafx.h"
#include "pstack.h"

void pStackClass::push(tokenClass token)
{
	StackCell *tempCell = new StackCell;
	tempCell->next = top;
	top = tempCell;
	tempCell->type = token.type;
	tempCell->subtype = token.subtype;
}