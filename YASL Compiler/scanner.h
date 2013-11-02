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
#include "general.h"
#include "table.h"

//Class used to store a token.
//The type and subtype should be constants defined in general.h.
class tokenClass
{ 
public:
	tokenClass();
	tokenClass(int type,int subtype,string lexeme);
	int type;
	int subtype;
	string lexeme;
	static string tokenIntToString(int tokenNameAsInt);//Map type/subtype int to its string representation
private:
}  ;

//Data structure used to group keyword with its associated type index.
class Keyword
{
public:
	Keyword(int type, const char* keyword);
	const char* keyword;
	int type;
};

//State used in the state matrix.
//It stores the action to be taken upon reading a new character.
//Final states with the action ACCEPT also has a tokenClass instance,
//which stores the information of the accepted token.
//Notice that memory for actionInfo and token may be
//allocated during initialization. The user of this class
//is responsible for releasing the memory by calling 
//delete on those two instances directly.
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
	tokenClass* token;
private:
};

//Class that scans a file and output
//tokens.
class scannerClass
{ 
public:
	scannerClass();
	//Return either a valid token or one with type EOF_T or EMPTY_T.
	//The former indicates EOF and the later indicates error.
	tokenClass getToken();
	void close();
	//Print out the state matrix for debugging.
	//An csv file will be created in the root folder.
	void printStateMatrix();
	void printCurrentLine();
	int numLinesProcessed();
	bool expressionDebugging;//Flag used by parser to decide whether to print expression debugging msg.
	tableClass symbolTable;
private:
	State stateMatrix[MAX_STATE][MAX_CHAR];
	fileManagerClass fileManager;
	void errorAndExit(string message);
	void buildStateMatrix();//Method used to build the state matrix.
	void buildStateMatrixCompact();//A slightly more efficient way to build the matrix. Less human-readable.
}  ;


#endif
