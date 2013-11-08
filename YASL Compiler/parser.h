//Tao Qian
//This file contains a class definition for an 
//operator precedence expression parser.

//The definitions of these routines can be found in parser.cpp

#ifndef _parser
#define _parser

#include "scanner.h"
#include "pstack.h"
#include <vector>

#define IDENTIFIER_TOKEN tokenClass(IDENTIFIER_T,NONE_ST,"identifier")
#define SEMICOLON_TOKEN tokenClass(SEMICOLON_T,NONE_ST,";")

class parserClass
{
public:
	parserClass();
	void parseProgram();
private:
	tokenClass t;
	fstream outfile;
	void parseStatement();
	void parseExpr();//Parse expressions use shift and reduce algorithm. If the expression is invalid, program will be terminated.
	void parseFollowID(SymbolNode* id);
	void parseFollowIf();
	int parseFollowExpr();
	void parseFollowBegin();
	void parseStatementTail();
	void parseFollowCin();
	void parseFollowCout();
	void parseCoutTail();
	void parseVarDecs();
	//try to parse either an int or boolean token. 
	//Returns the type of the symbol parsed.
	SymbolType tryParseType();
	void parseIdentList(SymbolType type);
	void parseIdentTail(SymbolType type);
	void parseFuncDecs();
	void parseFuncDecTail();
	void parseParamList();
	void parseTypeTail(SymbolType type);
	void parseFuncIdentTail();
	void parseBlock();
	void parseProgBody();

	Precedence precedenceTable[PRECEDENCE_TABLE_DIMENSION][PRECEDENCE_TABLE_DIMENSION];
	scannerClass scanner;
	//Build the precedence table, called in constructor.
	void buildPrecedenceTable();
	//Convert the token used by scanner to table index used in precedenceTable.
	int tokenToTableIndex(tokenClass token);
	//Set the precedence. The first two parameters are type index not table index.
	void setPre(int col,int row, Precedence pre);
	//Output precedence table to a csv file for debugging.
	void printPrecedenceTable();
	//Get the precedence of two tokens.
	Precedence prec(tokenClass firstToken,tokenClass secondToken);
	//Check whether the RHS is valid. 
	//It also checks whether the identifiers in the RHS is declared.
	bool isValidRHS(std::vector<tokenClass> tokens);
	//Exit the program when read an unexpected token for recursive descent parser.
	void recurDescentErrorAndExit(string found, vector<string> expected);
	//Exit the prorgam with the given message. Call this for syntax error.
	void errorAndExit(string message);
	//Check whether two token matches in the recursive descent parser.
	void checkTokenAndGetNext(tokenClass token, tokenClass expected);
	//Check whether a token is the end of an expression.
	bool isEndOfExpression(tokenClass token);

	//Check whether the identifier is already in the symbolTable.
	//if not, exit the program, otherwise return the symbol found.
	SymbolNode* checkId(string lexeme);

	string toAssemlyChar(char c);
};

#endif
