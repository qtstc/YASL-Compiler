//Tao Qian
//This file contains implementations of the 
//ScannerClass and TokenClass

//The class header is found in scanner.h

#include "stdafx.h"  // Required for visual studio to work correctly
#include "scanner.h"

TokenClass::TokenClass():type(EMPTY_T),subtype(EMPTY_ST),lexeme(EMPTY_LEXEME){}

TokenClass::TokenClass(int type, int subtype, string lexeme):type(type),subtype(subtype),lexeme(lexeme){}

State::State():nextStateNum(INVALID_STATE),action(NO_ACTION),token(NULL),actionInfo(NULL),needPushBack(false){}

string TokenClass::tokenIntToString(int tokenNameAsInt)
{
	switch(tokenNameAsInt)
	{
		case NONE_ST:
			return "NONE_ST";
		case ARITHM_T:
			return "ARITHM_T";
		case ADD_ST:
			return "ADD_ST";
		case SUBSTRACT_ST:
			return "SUBSTRACT_ST";
		case MULTIPLY_ST:
			return "MULTIPLY_ST";
			
		case RELOP_T:
			return "RELOP_T";
		case EQUAL_ST:
			return "EQUAL_ST";
		case UNEQUAL_ST:
			return "UNEQUAL_ST";
		case GREATER_ST:
			return "GREATER_ST";
		case GREATEROREQUAL_ST:
			return "GREATEROREQUAL_ST";
		case LESS_ST:
			return "LESS_ST";
		case LESSOREQUAL_ST:
			return "LESSOREQUAL_ST";
	
		case BITWISE_T:
			return "BITWISE_T";
		case BITLEFT_ST:
			return "BITLEFT_ST";
		case BITRIGHT_ST:
			return "BITRIGHT_ST";

		case PAREN_T:
			return "PAREN_T";
		case LEFTPAREN_ST:
			return "LEFTPAREN_ST";
		case RIGHTPAREN_ST:
			return "RIGHTPAREN_ST";

		case SEMICOLON_T:
			return "SEMICOLON_T";
		case COMMA_T:
			return "COMMA_T";
		case DOT_T:
			return "DOT_T";
		case TILDE_T:
			return "TILDE_T";
		case COMMENT_T:
			return "COMMENT_T";
		case STRING_T:
			return "STRING_T";
		case COMPILERDIR_T:
			return "COMPILERDIR_T";
		case IDENTIFIER_T:
			return "IDENTIFIER_T";
		case INTEGER_T:
			return "INTEGER_T";
		case ASSIGNMENT_T:
			return "ASSIGNMENT_T";
		default:
			return "Error";
	}
}

State::State(int nextStateNum):nextStateNum(nextStateNum),action(NO_ACTION),token(NULL),actionInfo(NULL),needPushBack(false){}

State::State(bool needPushBack,int type,int subtype,string lexeme):nextStateNum(0),needPushBack(needPushBack),token(new TokenClass(type,subtype,lexeme)),action(ACCEPT),actionInfo(NULL){}

State::State(Action errorOrWarningAction, string message):nextStateNum(0),action(errorOrWarningAction),needPushBack(false),token(NULL),actionInfo(new string(message)){}

State::State(int nextStateNum,Action sideAction):nextStateNum(nextStateNum),action(sideAction),needPushBack(false),token(NULL),actionInfo(NULL){}

std::ostream& operator<<(std::ostream &strm, const State &s) {
	if(s.token != NULL)
	{
		return  strm <<"{"<< s.nextStateNum <<"}"<<TokenClass::tokenIntToString(s.token->type)<<" "<<TokenClass::tokenIntToString(s.token->subtype)<<" "<<s.token->lexeme;
	}
	return strm <<"{"<< s.nextStateNum <<"}"<<"NULL";
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

	for(int i = 0;i<MAX_STATE;++i)
		for(int j = 0;j<MAX_CHAR;++j)
			stateMatrix[i][j] = State();

	//Read carriage return and whitespace at state 0 lead to 0
	State zeroState(0);
	stateMatrix[0]['\r'] = zeroState;
	stateMatrix[0][' '] = zeroState;

	//Read *-+.,()~ goes to the respective final state with action ACCEPT
	stateMatrix[0]['*'] = State(false,ARITHM_T,MULTIPLY_ST,"*");
	stateMatrix[0]['-'] = State(false,ARITHM_T,SUBSTRACT_ST,"-");
	stateMatrix[0]['+'] = State(false,ARITHM_T,ADD_ST,"+");
	stateMatrix[0]['.'] = State(false,DOT_T,NONE_ST,".");
	stateMatrix[0][','] = State(false,COMMA_T,NONE_ST,",");
	stateMatrix[0]['('] = State(false,PAREN_T,LEFTPAREN_ST,"(");
	stateMatrix[0][')'] = State(false,PAREN_T,RIGHTPAREN_ST,")");
	
	//Read "="
	int firstEqualStateNum = ++nonFinalStateNum;//State with number zero is reserved for the starting state
	stateMatrix[0]['='] = State(firstEqualStateNum);//Read first equal
	for(int i = 0;i<MAX_CHAR;++i)
	{
		if(i == '=')
				stateMatrix[firstEqualStateNum]['=']=State(false,ARITHM_T,EQUAL_ST,"==");
		else
			stateMatrix[firstEqualStateNum][i] = State(true,ASSIGNMENT_T,NONE_ST,"=");
	}

	//Read "<"
	int firstLessThanStateNum = ++nonFinalStateNum;
	stateMatrix[0]['<'] = State(firstLessThanStateNum);
	for(int i = 0;i<MAX_CHAR;++i)
	{
		if(i == '<')
			stateMatrix[firstLessThanStateNum]['<'] = State(false,BITWISE_T,BITLEFT_ST,"<<");
		else if(i=='>')
			stateMatrix[firstLessThanStateNum]['>'] = State(false,RELOP_T,UNEQUAL_ST,"<>");
		else if (i == '=')
			stateMatrix[firstLessThanStateNum]['='] = State(false,RELOP_T,LESSOREQUAL_ST,"<=");
		else
			stateMatrix[firstLessThanStateNum][i] = State(true,RELOP_T,LESS_ST,"<");
	}

	//Read ">"
	int firstGreaterThanStateNum = ++nonFinalStateNum;
	stateMatrix[0]['>']=State(firstGreaterThanStateNum);
	for(int i = 0;i<MAX_CHAR;++i)
	{
		if(i == '>')
			stateMatrix[firstLessThanStateNum]['<'] = State(false,BITWISE_T,BITRIGHT_ST,">>");
		else if (i == '=')
			stateMatrix[firstLessThanStateNum]['='] = State(false,RELOP_T,GREATEROREQUAL_ST,">=");
		else
			stateMatrix[firstGreaterThanStateNum][i] = State(true,RELOP_T,LESS_ST,">");
	}
	
	//Read digit
	int firstDigitStateNum = ++nonFinalStateNum;
	string digits = "0123456789";
	for(int i = 0;i<digits.length();++i)
	{
		stateMatrix[0][digits[i]] = State(firstDigitStateNum);
	}
	for(int i = 0;i<MAX_CHAR;++i)
	{
		if(i<= '9' && i >='0')
			stateMatrix[firstDigitStateNum][i] = State(firstDigitStateNum);
		else
			stateMatrix[firstDigitStateNum][i] = State(true,INTEGER_T,NONE_ST,"int");
	}

	//Read string
	int leftQuoteStateNum = ++nonFinalStateNum;
	stateMatrix[0]['\''] = State(leftQuoteStateNum);
	for(int i =0;i<MAX_CHAR;++i)
	{
		if(i == '\'')
			stateMatrix[leftQuoteStateNum][i] = State(false,STRING_T,NONE_ST,"string");
		else if (i == EOF_INDEX)
			stateMatrix[leftQuoteStateNum][i] = State(ERROR,"Single quote expected at the end of string.");
		else
			stateMatrix[leftQuoteStateNum][i]=State(leftQuoteStateNum);
	}

	//Read double slash started comment
	int firstSlashStateNum = ++nonFinalStateNum;
	int secondSlashStateNum = ++nonFinalStateNum;
	stateMatrix[0]['/']=State(firstSlashStateNum);
	for(int i = 0;i<MAX_CHAR;++i)
	{
		if(i == '/')
			stateMatrix[firstSlashStateNum][i] = State(secondSlashStateNum);
		else 
			stateMatrix[firstSlashStateNum][i] = State(ERROR,"Comment should be started by double slashes.");
	}
	for(int i = 0;i<MAX_CHAR;i++)
	{
		if(i == '\r')
			stateMatrix[secondSlashStateNum][i] = State(0);
		else
			stateMatrix[secondSlashStateNum][i] = State(secondSlashStateNum,CLEAR_BUFFER);
	}

	//Read identifier
	int firstLetterStateNum = ++nonFinalStateNum;
	string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	for(int i = 0;i<letters.length();++i)
		stateMatrix[0][letters[i]]=State(firstLetterStateNum);
	for(int i = 0;i<MAX_CHAR;++i)
	{
		if(('A'<=i&&i<='z')||('0'<=i&&i<='9')||(i=='_'))
			stateMatrix[firstLetterStateNum][i] = State(firstLetterStateNum);
		else 
			stateMatrix[firstLetterStateNum][i] = State(true,IDENTIFIER_T,NONE_ST,"identifier");
	}

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

void ScannerClass::close()
{
	fileManager.closeSourceProgram();
	for(int i = 0;i<MAX_STATE;i++)
		for(int j = 0;j<MAX_CHAR;j++)
		{
			delete stateMatrix[i][j].actionInfo;
			delete stateMatrix[i][j].token;
		}
}
