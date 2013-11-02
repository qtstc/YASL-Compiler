//Tao Qian
//This file contains implementations of the 
//tableClass and symbolClass

//The class header is found in table.h

#include "stdafx.h"
#include "table.h"


SymbolNode::SymbolNode(string lexeme, SymbolKind kind, SymbolType type):lexeme(lexeme),kind(kind),type(type),parameterTop(NULL),next(NULL)
{}

void SymbolNode::addParameter(SymbolNode* parameter)
{
	//First making sure the symbol is a function.
	if(kind != FUNC_ID)
		throw exception("Adding parameter to a non-function symbol.");
	if(parameter->kind != REF_PARAM && parameter-> kind!= VALUE_PARAM)
		throw exception("Symbol kind is neither REF_PARAM nor VALUE_PARAM.");
	parameter->nestingLevel = nestingLevel;
	int offset = 0;
	if(parameterTop == NULL)
	{
		parameterTop = parameter;
		parameter->offset = offset;
		return;
	}
	//Add the new paramter to the end of the list,
	//so the sequence at which the paramter is added
	//will be the same as the sequence at which
	//the parameter appears in the function definition.
	SymbolNode* end = parameterTop;
	offset++;
	while(end->next != NULL)
	{
		end = end->next;
		offset++;
	}
	parameter->offset = offset;
	end->next = parameter;
}
SymbolNode::~SymbolNode()
{
	SymbolNode* p = parameterTop;
	while(p != NULL)
	{
		SymbolNode* temp = p;
		p = p->next;
		delete temp;
	}
}

string SymbolNode::toString()
{
	string s = toStringForParameter();
	s += ", nesting level=";
	s += to_string(nestingLevel);
	if(parameterTop != NULL)
	{
		s+="\n\tparameters:";
		SymbolNode *p = parameterTop;
		while(p != NULL)
		{
			s += "\n\t"+p->toStringForParameter();
		    p = p->next;
		}	
	}
	return s;
}

string SymbolNode::toStringForParameter()
{
	string s = "lexeme=";
	s += lexeme;
	s += ", kind=";
	s += symbolKindStrings[kind];
	s += ", type=";
	s += symbolTypeStrings[type];
	s += ", offset=";
	s += to_string(offset);
	return s;
}

TableLevel::TableLevel(string name, int nestingLevel):name(name),nestingLevel(nestingLevel),nextOffset(0),top(NULL),next(NULL)
{}

bool TableLevel::addSymbol(SymbolNode* node)
{
	if(lookup(node->lexeme) != NULL)
		return false;
	node->nestingLevel = nestingLevel;
	node->offset = nextOffset;
	if(node->kind != FUNC_ID)
		nextOffset++;
	//Just add to the front.
	node->next = top;
	top = node;
	return true;
}

SymbolNode* TableLevel::lookup(string lexeme)
{
	SymbolNode *p = top;
	while(p!=NULL)
	{
		if(p->lexeme == lexeme)
			return p;
		p = p->next;
	}
	return NULL;
}

TableLevel::~TableLevel()
{
	SymbolNode* p = top;
	while(p != NULL)
	{
		SymbolNode* temp = p;
		p = p->next;
		delete temp;
	}
}

string TableLevel::toString()
{
	string s = "name=";
	s += name;
	s += ", nesting level=";
	s += to_string(nestingLevel);
	if(top != NULL)
	{
		SymbolNode *p = top;
		while(p != NULL)
		{
			s += "\n"+p->toString();
			p = p->next;
		}
	}
	return s;
}

tableClass::tableClass():top(NULL),nextNestingLevel(0)
{}

void tableClass::tableAddLevel(string scope)
{
	string currentScope = "";
	if(top != NULL)
		currentScope = top->name;
	TableLevel * level = new TableLevel(currentScope+"."+scope,nextNestingLevel++);
	level -> next = top;
	top = level;
}

void tableClass::tableDelLevel()
{
	if(top == NULL)
		throw exception("Cannot delete because top level is already null");
	TableLevel* temp = top;
	top = top->next;
	delete temp;
	nextNestingLevel--;
}

bool tableClass::tableAddEntry(SymbolNode* node)
{
	if(top == NULL)
		throw exception("Cannot add symbol to the table because no level is created yet.");
	return top->addSymbol(node);
}

SymbolNode* tableClass::tableLookup(string lexeme)
{
	TableLevel *p = top;
	while(p != NULL)
	{
		SymbolNode* levelResult = p->lookup(lexeme);
		if(levelResult != NULL)
			return levelResult;
		p = p->next;
	}
	return NULL;
}

string tableClass::toString()
{
	string s = "*******************************************";
	TableLevel *p = top;
	while(p != NULL)
	{
		s += "\n" + p->toString();
		p= p->next;
	}
	return s;
}

void tableClass::addFunctionParameter(SymbolNode* parameter)
{
	if(top == NULL || top->next == NULL)
		throw exception("Cannot add parameter because scope table is missing.");
	TableLevel* lastLevel = top->next;
	if(lastLevel->top == NULL)
		throw exception("Cannot add parameter because function identifier is missing in the table.");
	lastLevel->top->addParameter(parameter);
}

tableClass::~tableClass()
{
	while(top != NULL)
		tableDelLevel();
}