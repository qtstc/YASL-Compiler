//Tao Qian
//This file contains a class definition for an 
//operator precedence expression parser.

//The definitions of these routines can be found in parser.cpp

#ifndef _parser
#define _parser

#include "scanner.h"

class parserClass
{
public:
	parserClass();
	void parseExpr();
private:
	Precedence precedenceTable[PRECEDENCE_TABLE_DIMENSION][PRECEDENCE_TABLE_DIMENSION];
	scannerClass scanner;
	void buildPrecedenceTable();
	int typeToTableIndex(int type);
	void setPre(int col,int row, Precedence pre);
	void printPrecedenceTable();
};

#endif
