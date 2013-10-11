//Tao Qian
//This file contains a class definition for an 
//operator precedence expression parser.

//The definitions of these routines can be found in parser.cpp

#ifndef _parser
#define _parser

#include "scanner.h"
#include "pstack.h"
#include <vector>

class parserClass
{
public:
	parserClass();
	void parseProgram();
private:
	//For project 4
	tokenClass t;
	void parseStatement();
	void parseExpr();//Parse expressions use shift and reduce algorithm. If the expression is invalid, program will be terminated.

	Precedence precedenceTable[PRECEDENCE_TABLE_DIMENSION][PRECEDENCE_TABLE_DIMENSION];
	scannerClass scanner;
	void buildPrecedenceTable();//Build the precedence table, called in constructor.
	int typeToTableIndex(int type);//Convert the type index used by scanner to table index used in precedenceTable.
	void setPre(int col,int row, Precedence pre);//Set the precedence. The first two parameters are type index not table index.
	void printPrecedenceTable();//Output precedence table to a csv file for debugging.
	Precedence prec(int firstType,int secondType);//Get the precedence. The parameters are type index.
	Precedence prec(tokenClass firstToken,tokenClass secondToken);//Get the precedence of two tokens.
	bool isValidRHS(std::vector<tokenClass> tokens);//Check whether the RHS is valid.
	void recurDescentErrorAndExit(string found, vector<string> expected);//Exit the program when read an unexpected token for recursive descent parser.
	void errorAndExit(string message);//Exit the prorgam with the given message. Call this for syntax error.
	void checkTokenAndGetNext(tokenClass token, tokenClass expected);//Check whether two token matches in the recursive descent parser.
	bool isEndOfExpression(tokenClass token);//Check whether a token is the end of an expression.
	bool isEndOfExpression(int type);//Check whether token is the end of an expression.
};

#endif
