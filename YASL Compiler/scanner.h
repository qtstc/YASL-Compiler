//Tao Qian
//This file contains a class definition for scanning the
//file containing the YASL source program to be compiled.

//The definitions of these routines can be found in scanner.cpp

#ifndef _scanner
#define _scanner

#include "filemngr.h"
#include <iostream>
#include <fstream>
#include <string>

#define EMPTY_T -10
#define EMPTY_ST -11
#define EMPTY_LEXEME ""

#define NONE_ST 1

#define ARITHM_T 10
#define ADD_ST 11
#define SUBSTRACT_ST 12
#define MULTIPLY_ST 13

#define RELOP_T 20
#define EQUAL_ST 21
#define UNEQUAL_ST 22
#define GREATEROREQUAL_ST 23
#define LESSOREQUAL_ST 24
#define GREATER_ST 25
#define LESS_ST 26

#define BITWISE_T 30
#define BITLEFT_ST 31
#define BITRIGHT_ST 32

#define PAREN_T 40
#define LEFTPAREN_ST 41
#define RIGHTPAREN_ST 42

#define SEMICOLON_T 50
#define COMMA_T 60
#define DOT_T 70
#define TILDE_T 80
#define STRING_T 90
#define IDENTIFIER_T 100
#define INTEGER_T 110
#define ASSIGNMENT_T 120
#define EOF_T 130

#define MAX_STATE 100
#define MAX_CHAR 129//EOF is -1. We store it as 128.
#define EOF_INDEX 128

#define INVALID_STATE -1

enum Action {NO_ACTION,ACCEPT,ERROR,WARNING,CLEAR_BUFFER,CHECK_COMPILER_DIRECTIVE};

class TokenClass
{ 
public:
	TokenClass();
	TokenClass(int type,int subtype,string lexeme);
	int type;
	int subtype;
	string lexeme;
	static string tokenIntToString(int tokenNameAsInt);
private:
}  ;

class State
{
public:
	State();
	State(int nextStateNum);
	State(int nextStateNum,bool needPushBack);
	State(bool needPushBack,int type,int subtype,string lexeme);
	State(string errorMessage);
	State(int nextStateNum,Action sideAction);
	friend ostream& operator<<(std::ostream &strm, const State &s);
	int nextStateNum;
	Action action;
	bool needPushBack;
	string* actionInfo;
	TokenClass* token;
private:
};

class ScannerClass
{ 
public:
	ScannerClass();
	TokenClass getToken();
	void close();
	State stateMatrix[MAX_STATE][MAX_CHAR];
	void printStateMatrix();
private:
	fileManagerClass fileManager;
	void buildStateMatrix();
}  ;


#endif
