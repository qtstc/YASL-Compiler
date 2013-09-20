//Tao Qian
//This file contains a class definition for scanning the
//file containing the YASL source program to be compiled.

//The definitions of these routines can be found in scanner.cpp

#include "filemngr.h"
#include <iostream>
#include <fstream>

#ifndef _scanner
#define _scanner

#define EMPTY_T -1
#define EMPTY_ST -1
#define EMPTY_LEXEME ""

#define MAX_STATE 200
#define MAX_CHAR 129//EOF is -1. We store it as 128.

#define INVALID_STATE -1;

class TokenClass
{ public:
	TokenClass();
	TokenClass(int type,int subtype,string lexeme);

  private:
	  int type;
	  int subtype;
	  string lexeme;
}  ;

class ScannerClass
{ public:
	ScannerClass();
	TokenClass getToken();
	void close();
	int stateMatrix[MAX_STATE][MAX_CHAR];
	void printStateMatrix();
  private:
	  //fileManagerClass fileManager;
	  string lexeme;
	  void buildStateMatrix();
}  ;

#endif
