//Tao Qian
//This file contains implementations of the 
//ScannerClass and TokenClass

//The class header is found in scanner.h

#include "stdafx.h"  // Required for visual studio to work correctly
#include "scanner.h"

TokenClass::TokenClass():type(EMPTY_T),subtype(EMPTY_ST),lexeme(EMPTY_LEXEME){}

TokenClass::TokenClass(int type, int subtype, string lexeme):type(type),subtype(subtype),lexeme(lexeme){}

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
		case STRING_T:
			return "STRING_T";
		case IDENTIFIER_T:
			return "IDENTIFIER_T";
		case INTEGER_T:
			return "INTEGER_T";
		case ASSIGNMENT_T:
			return "ASSIGNMENT_T";
		case EOF_T:
			return "EOF_T";
		default:
			return "Error";
	}
}

State::State():nextStateNum(INVALID_STATE),action(NO_ACTION),token(NULL),actionInfo(NULL),needPushBack(false){}

State::State(int nextStateNum):nextStateNum(nextStateNum),action(NO_ACTION),token(NULL),actionInfo(NULL),needPushBack(false){}

State::State(bool needPushBack,int type,int subtype,string lexeme):nextStateNum(0),needPushBack(needPushBack),token(new TokenClass(type,subtype,lexeme)),action(ACCEPT),actionInfo(NULL){}

State::State(string errorMessage):nextStateNum(EOF_INDEX),action(ERROR),needPushBack(false),token(NULL),actionInfo(new string(errorMessage)){}

State::State(int nextStateNum,Action sideAction):nextStateNum(nextStateNum),action(sideAction),needPushBack(false),token(NULL),actionInfo(NULL){}

State::State(int nextStateNum,bool needPushBack):nextStateNum(nextStateNum),action(NO_ACTION),needPushBack(needPushBack),token(NULL),actionInfo(NULL){}

std::ostream& operator<<(std::ostream &strm, const State &s) {
	if(s.token != NULL)
	{
		return  strm <<"{"<< s.nextStateNum <<"}"<<TokenClass::tokenIntToString(s.token->type)<<" "<<TokenClass::tokenIntToString(s.token->subtype)<<" "<<s.token->lexeme;
	}
	return strm <<"{"<< s.nextStateNum <<"}"<<"NULL";
}


ScannerClass::ScannerClass()
{
	buildStateMatrix();
}

TokenClass ScannerClass::getToken()
{
	int currentStateNum = 0;
	int c;
	string currentLexeme="";
	do
	{
		c = (int)fileManager.getNextChar();
		if(c == EOF)//Because EOF(-1) is not a valid index, we need to change it before use it in the matrix.
			c = EOF_INDEX;

		State s = stateMatrix[currentStateNum][c];//Get the next state.
		if(s.nextStateNum == INVALID_STATE)//If state is invalid
		{
			cout<<"Invalid state"<<endl;
			return TokenClass(EOF_T,NONE_ST,"error");
		}

		if(s.nextStateNum == EOF_INDEX)//If reached EOF
		{
			cout<<"EOF"<<endl;
			return TokenClass(EOF_T,NONE_ST,"EOF");
		}

		switch(s.action)
		{
		case NO_ACTION:
			currentLexeme += c;
			break;
		case ACCEPT:
				if(!s.needPushBack)
					currentLexeme += c;
				else
					fileManager.pushBack();
				cout<<"accepted"<<endl;
				return TokenClass(s.token->type,s.token->subtype,currentLexeme);
		case WARNING:
			cout<<"warning"<<endl;
			break;
		case ERROR:
			cout<<"error"<<endl;
			return TokenClass(EOF_T,NONE_ST,"error");
		case CLEAR_BUFFER:
			currentLexeme = "";
			break;
		case CHECK_COMPILER_DIRECTIVE:
			cout<<"compiler directive"<<endl;
			break;
		}

		currentStateNum = s.nextStateNum;
		if(currentStateNum == 0)
			currentLexeme = "";
	}
	while(true);

	return TokenClass(EMPTY_T,EMPTY_ST,EMPTY_LEXEME);
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
	stateMatrix[0]['\n'] = zeroState;
	stateMatrix[0][' '] = zeroState;
	stateMatrix[0][EOF_INDEX] = State(EOF_INDEX);//End of file indicator

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
			stateMatrix[firstDigitStateNum][i] = State(true,INTEGER_T,NONE_ST,"int_lexeme");
	}

	//Read string
	int leftQuoteStateNum = ++nonFinalStateNum;
	stateMatrix[0]['\''] = State(leftQuoteStateNum);
	for(int i =0;i<MAX_CHAR;++i)
	{
		if(i == '\'')
			stateMatrix[leftQuoteStateNum][i] = State(false,STRING_T,NONE_ST,"string_lexeme");
		else if (i == EOF_INDEX)
			stateMatrix[leftQuoteStateNum][i] = State("Single quote expected at the end of string.");
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
			stateMatrix[firstSlashStateNum][i] = State("Comment should be started by double slashes.");
	}
	for(int i = 0;i<MAX_CHAR;i++)
	{
		if(i == '\r'||i == '\n')
			stateMatrix[secondSlashStateNum][i] = State(0);
		else if(i == EOF_INDEX)
			stateMatrix[secondSlashStateNum][i] = State(0,true);
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
			stateMatrix[firstLetterStateNum][i] = State(true,IDENTIFIER_T,NONE_ST,"identifier_lexeme");
	}

	//Read comment enclosed in brace
	int leftBraceStateNum = ++nonFinalStateNum;
	int secondDollarStateNum = ++nonFinalStateNum;
	int thirdLetterStateNum = ++nonFinalStateNum;
	int fourthAddMinusStateNum = ++nonFinalStateNum;
	int bracedCommentStateNum = ++nonFinalStateNum;
	stateMatrix[0]['{'] = State(leftBraceStateNum);
	for(int i = 0;i<MAX_CHAR;++i)
	{
		if(i == '}')
			stateMatrix[leftBraceStateNum][i] = State(0);
		else if(i == '$')
			stateMatrix[leftBraceStateNum][i] = State(secondDollarStateNum);
		else if(i == EOF_INDEX)
			stateMatrix[leftBraceStateNum][i] = State("Comment or compiler directive needs to be ended with a right brace.");
		else 
			stateMatrix[leftBraceStateNum][i] = State(bracedCommentStateNum);

		if(i == '}')
			stateMatrix[secondDollarStateNum][i] = State(0);
		else if (i <= 'z' && i >= 'A')
			stateMatrix[secondDollarStateNum][i] = State(thirdLetterStateNum);
		else if (i == EOF_INDEX)
			stateMatrix[secondDollarStateNum][i] = State("Comment or compiler directive needs to be ended with a right brace.");
		else
			stateMatrix[secondDollarStateNum][i] = State(bracedCommentStateNum);

		if(i == '}')
			stateMatrix[thirdLetterStateNum][i] = State(0);
		else if(i == '+' ||i == '-')
			stateMatrix[thirdLetterStateNum][i] = State(fourthAddMinusStateNum);
		else if (i == EOF_INDEX)
			stateMatrix[thirdLetterStateNum][i] = State("Comment or compiler directive needs to be ended with a right brace.");
		else 
			stateMatrix[thirdLetterStateNum][i] = State(bracedCommentStateNum);

		if(i == '}')
			stateMatrix[fourthAddMinusStateNum][i] = State(0,CHECK_COMPILER_DIRECTIVE);
		else if(i == EOF_INDEX)
			stateMatrix[fourthAddMinusStateNum][i] = State("Comment or compiler directive needs to be ended with a right brace.");
		else stateMatrix[fourthAddMinusStateNum][i] =State(bracedCommentStateNum);

		if(i == '}')
			stateMatrix[bracedCommentStateNum][i] = State(0);
		else if(i == EOF_INDEX)
			stateMatrix[bracedCommentStateNum][i] = State("Comment or compiler directive needs to be ended with a right brace.");
		else
			stateMatrix[bracedCommentStateNum][i] = State(bracedCommentStateNum,CLEAR_BUFFER);
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
