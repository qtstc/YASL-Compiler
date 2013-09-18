//Tao Qian
//This file contains a class definition for scanning the
//file containing the YASL source program to be compiled.

//The definitions of these routines can be found in scanner.cpp

#ifndef _scanner
#define _scanner
class TokenClass
{ public:

  private:
	  int type;
	  int subtype;
	  string lexeme;
}  ;

class ScannerClass
{ public:
	TokenClass getToken();
	void close();
  private:
	  fileManagerClass fileManager;
	  string currentLexeme;
}  ;

#endif
