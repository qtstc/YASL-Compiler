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
	tokenClass t;
	fstream outfile;
	void parseStatement();
	SymbolType parseExpr();//Parse expressions use shift and reduce algorithm. If the expression is invalid, program will be terminated.
	void parseFollowID(SymbolNode* id);
	void parseFollowIf();
	int parseFollowExpr();
	void parseFollowBegin();
	void parseStatementTail();
	void parseFollowCin(bool onlyParam);
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
	void parseBlock(int paramCount);
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
	//Check whether the RHS is valid, if it is, a pointer to a node in the SymbolTable will be returned.
	//It also checks whether the identifiers in the RHS is declared.
	SymbolNode* isValidRHS(std::vector<tokenSymbolClass> tokens);
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

	//Method used for generating PAL code.
	//Convert ASCII char to the ones used in PAL
	string toPALChar(char c);
	//Print a PAL binary instruction. If the pointers are not null, the offsets of the symbols will be used
	//as the address of the PAL parameter. Otherwise, the strings will be the operands.
	void printInstruction(string instruction, SymbolNode* firstParamPtr, string firstParam, SymbolNode* secondParamPtr, string secondParam);
	//Print a PAL unary instruction
	void printInstruction(string instruction, SymbolNode* firstParamPtr, string firstParam);
	//Print branch name in PAL
	void printBranch(string branchName);
	//Convert an int to PAL literal
	string toPALLiteral(int n);
	//Convert an int(in string) to PAL literal
	string toPALLiteral(string n);
	//Get the name of the next temporary variable to be used in the symbol table.
	string getNextTempName();
	//Add a temporary variable to the symbol table. Also generate PAL code that reserves space for the temp variable on the stack.
	SymbolNode* addTempVariable(SymbolType type);
	//Get the direct addressing reference to the given memory location
	string toPALDirectAddressing(string memoryLocation);
	//Get the PAL parameter either based on the offset of the symbol,
	//or use the string.
	string getParameter(SymbolNode* paramPtr, string param);
	//Check that a variable is either an integer or a boolean.
	//Throw an error otherwise.
	void checkVariable(SymbolType type);
	//Check that a variable is either an integer or a boolean.
	//Throw an error otherwise.
	void checkVariable(SymbolNode* node);
	//Pal operations
	SymbolNode* palArithmetic(SymbolNode* node1, SymbolNode* node2,string op);
	SymbolNode* palMod(SymbolNode* node1, SymbolNode* node2);
	SymbolNode* palAnd(SymbolNode* node1,SymbolNode* node2);
	SymbolNode* palOr(SymbolNode* node1,SymbolNode* node2);
	SymbolNode* palBranch(SymbolNode* node1, SymbolNode* node2, string op);
};

#endif
