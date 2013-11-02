//Tao Qian
//This file contains a class definition for tableClass and symbolClass

//The definitions of these routines can be found in table.cpp


#ifndef _pstack
#define _pstack

#include "scanner.h"

class SymbolNode
{
public:
	string lexeme;
	SymbolKind kind;
	SymbolType type;
	//The offset of this identifier from the start of the current scope
	int offset;
	//The nesting level of the table that this entry is contained in.
	int nestingLevel;
	//If (kind == FUNC_ID) then store a pointer to 
	//a linked list of the formal parameters for the function.
	SymbolNode *parameterTop;
};

class TableLevel
{
public:
	//The name of the scoping level (for example, main.foo.fee).
	string name;
	/*
	The nesting level of the table 
	(level zero for program body, level one for everything defined within the program body, 
	level two for those things defined inside of level 1 scopes, etc.)
	*/
	int nestingLevel;
	/*
	The next offset to be used when a new entry is added to this level of the stack.
	This offset should start at 0 when the stack level is created.
	The offset should be incremented by 1 each time a variable identifier 
	is added to this level of the table.
	*/
	int nextOffset;
	/*
	A pointer to a self-organizing list of nodes.  
	Each node contains information about a symbol.  
	*/
	SymbolNode *top;
};

class tableClass
{
public:
	tableClass();
	~tableClass();
	/*
	Add a new level to the table.  
	The new level will contain an empty (self-organizing) list.  
	This routine is used when you enter the scope of a new function.
	*/
	void tableAddLevel();
	/*
	Delete the top-most level of the table. 
	Free all memory.  
	Used when you leave the scope of a new function.
	*/
	void tableDelLevel();
	/*
	Add an entry to the table 
	(the entry is added to the front of the self-organizing list 
	which is the top-most table on the stack of tables.)  
	*/
	void tableAddEntry(SymbolNode* Node);
	/*
	Given a lexeme as a parameter, 
	determine if it appears in the table 
	(searching multiple levels may be performed.) 
	*/
	SymbolNode* tableLookup(string lexeme);
private:
	TableLevel *top;

};

#endif