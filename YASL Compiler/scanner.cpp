//Tao Qian
//This file contains implementations of the 
//ScannerClass and TokenClass

//The class header is found in scanner.h

#include "stdafx.h"  // Required for visual studio to work correctly
#include "scanner.h"

TokenClass::TokenClass():type(EMPTY_T),subtype(EMPTY_ST),lexeme(EMPTY_LEXEME){}

TokenClass::TokenClass(int type, int subtype, string lexeme):type(type),subtype(subtype),lexeme(lexeme){}

State::State():nextStateNum(INVALID_STATE),action(NO_ACTION),token(NULL),actionInfo(NULL),needPushBack(false){}

State::State(int nextStateNum):nextStateNum(nextStateNum),action(NO_ACTION),token(&TokenClass(0,0,"")),actionInfo(NULL),needPushBack(false){}

State::State(int nextStateNum,bool needPushBack,int type,int subtype,string lexeme):nextStateNum(nextStateNum),needPushBack(needPushBack),action(ACCEPT),token(&TokenClass(type,subtype,lexeme)),actionInfo(NULL){}

std::ostream& operator<<(std::ostream &strm, const State &s) {
	string tokenString = "NULL";
	if(s.token != NULL)
	{
		tokenString = s.token->type;
		tokenString += " ";
		tokenString += s.token->subtype;
		tokenString += " ";
		tokenString += s.token->lexeme;
	}
	return strm <<"("<< s.nextStateNum <<")"<<tokenString;
}


ScannerClass::ScannerClass():lexeme(EMPTY_LEXEME)
{
	buildStateMatrix();
}

TokenClass ScannerClass::getToken()
{
	//char c = fileManager.getNextChar();
	TokenClass token(0,0,"");
	return token;
}

void ScannerClass::buildStateMatrix()
{
	int nonFinalStateNum = 0;

	for(int i = 0;i<MAX_STATE;i++)
		for(int j = 0;j<MAX_CHAR;j++)
			stateMatrix[i][j] = State();

	//Read carriage return and whitespace at state 0 lead to 0
	State zeroState(0);
	stateMatrix[0]['\r'] = zeroState;
	stateMatrix[0][' '] = zeroState;

	//Read *-+.,()~ goes to the respective final state with action ACCEPT
	stateMatrix[0]['*'] = State(0,false,ARITHM_T,MULTIPLY_ST,"*");
	stateMatrix[0]['-'] = State(0,false,ARITHM_T,SUBSTRACT_ST,"-");
	stateMatrix[0]['+'] = State(0,false,ARITHM_T,ADD_ST,"+");
	stateMatrix[0]['.'] = State(0,false,DOT_T,NONE_ST,".");
	stateMatrix[0][','] = State(0,false,COMMA_T,NONE_ST,",");
	stateMatrix[0]['('] = State(0,false,PAREN_T,LEFTPAREN_ST,"(");
	stateMatrix[0][')'] = State(0,false,PAREN_T,RIGHTPAREN_ST,")");
	
	//Read "="
	int firstEqualStateNum = ++nonFinalStateNum;//State with number zero is reserved for the starting state
	stateMatrix[0]['='] = State(firstEqualStateNum);//Read first equal
	for(int i = 0;i<MAX_CHAR;i++)
		stateMatrix[firstEqualStateNum][i] = State(0,true,ASSIGNMENT_T,NONE_ST,"=");
	stateMatrix[firstEqualStateNum]['=']=State(0,false,ARITHM_T,EQUAL_ST,"==");

	//Read "<"
	int firstLessThanStateNum = ++nonFinalStateNum;
	stateMatrix[0]['<'] = State(firstLessThanStateNum);
	for(int i = 0;i<MAX_CHAR;i++)
		stateMatrix[firstLessThanStateNum][i] = State(0,true,RELOP_T,LESS_ST,"<");
	stateMatrix[firstLessThanStateNum]['<'] = State(0,false,BITWISE_T,BITLEFT_ST,"<<");
	stateMatrix[firstLessThanStateNum]['>'] = State(0,false,RELOP_T,UNEQUAL_ST,"<>");
	stateMatrix[firstLessThanStateNum]['='] = State(0,false,RELOP_T,LESSOREQUAL_ST,"<=");

	//Read ">"
	int firstGreaterThanStateNum = ++nonFinalStateNum;
	stateMatrix[0]['>']=State(firstGreaterThanStateNum);
	for(int i = 0;i<MAX_CHAR;i++)
		stateMatrix[firstGreaterThanStateNum][i] = State(0,true,RELOP_T,LESS_ST,">");
	stateMatrix[firstLessThanStateNum]['<'] = State(0,false,BITWISE_T,BITRIGHT_ST,">>");
	stateMatrix[firstLessThanStateNum]['='] = State(0,false,RELOP_T,GREATEROREQUAL_ST,">=");
}

void ScannerClass::printStateMatrix()
{
	ofstream myfile;
	myfile.open ("stateMatrix.csv");

	for(int i = 0;i<MAX_STATE;i++)
	{
		for(int j = 0;j<MAX_CHAR;j++)
			myfile<<stateMatrix[i][j]<<",";
		myfile<<"\n";
	}
	myfile.close();
}
