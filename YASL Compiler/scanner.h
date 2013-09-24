//Tao Qian
//This file contains a class definition for scanning the
//file containing the YASL source program to be compiled.

//The definitions of these routines can be found in scanner.cpp

#include "filemngr.h"
#include <iostream>
#include <fstream>
#include <string>

#ifndef _scanner
#define _scanner

#define EMPTY_T -1
#define EMPTY_ST -2
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
#define COMMENT_T 90
#define STRING_T 100
#define COMPILERDIR_T 110
#define IDENTIFIER_T 120
#define INTEGER_T 130
#define ASSIGNMENT_T 140

#define MAX_STATE 100
#define MAX_CHAR 129//EOF is -1. We store it as 128.

#define INVALID_STATE -1

enum Action {NO_ACTION,PUSH_BACK,ACCEPT,ERROR,WARNING};

class TokenClass
{ 
public:
	TokenClass();
	TokenClass(int type,int subtype,string lexeme);
	int type;
	int subtype;
	string lexeme;
private:
}  ;

class State
{
public:
	State();
	State(int nextStateNum);
	State(int nextStateNum,bool needPushBack,int type,int subtype,string lexeme);
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
	//fileManagerClass fileManager;
	string lexeme;
	void buildStateMatrix();
}  ;


#endif
