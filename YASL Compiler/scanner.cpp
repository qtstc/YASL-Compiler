//Tao Qian
//This file contains implementations of the 
//scannerClass, tokenClass and State class.

//The class header is found in scanner.h

#include "stdafx.h"  // Required for visual studio to work correctly
#include "scanner.h"
#include <string.h>

tokenClass::tokenClass():type(EMPTY_T),subtype(EMPTY_ST),lexeme(EMPTY_LEXEME){}

tokenClass::tokenClass(int type, int subtype, string lexeme):type(type),subtype(subtype),lexeme(lexeme){}

string tokenClass::tokenIntToString(int tokenNameAsInt)
{
	switch(tokenNameAsInt)
	{
	case EMPTY_T:
		return "EMPTY_T";
	case EMPTY_ST:
		return "EMPTY_ST";

	case NONE_ST:
		return "NONE_ST";
	case ADDOP_T:
		return "ADDOP_T";
	case ADD_ST:
		return "ADD_ST";
	case SUBSTRACT_ST:
		return "SUBSTRACT_ST";
	case OR_ST:
		return "OR_ST";

	case MULOP_T:
		return "MULOP_T";
	case MULTIPLY_ST:
		return "MULTIPLY_ST";
	case AND_ST:
		return "AND_ST";
	case DIV_ST:
		return "DIV_ST";
	case MOD_ST:
		return "MOD_ST";

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

	case LEFTPAREN_T:
		return "LEFTPAREN_T";
	case RIGHTPAREN_T:
		return "RIGHTPAREN_T";

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
	case AMPERSAND_T:
		return "AMPERSAND_T";

	case PROGRAM_T:
		return "PROGRAM_T";
	case FUNCTION_T:
		return "FUNCTION_T";
	case BEGIN_T:
		return "BEGIN_T";
	case END_T:
		return "END_T";
	case IF_T:
		return "IF_T";
	case THEN_T:
		return "THEN_T";
	case ELSE_T:
		return "ELSE_T";
	case WHILE_T:
		return "WHILE_T";
	case DO_T:
		return "DO_T";
	case COUT_T:
		return "COUNT_T";
	case CIN_T:
		return "CIN_T";
	case ENDL_T:
		return "ENDL_T";
	case INT_T:
		return "INT_T";
	case BOOLEAN_T:
		return "BOOLEAN_T";
	case TRUE_T:
		return "TRUE_T";
	case FALSE_T:
		return "FALSE_T";

	case EOF_T:
		return "EOF_T";
	default:
		return "Token index not recognized";
	}
}

State::State():nextStateNum(INVALID_STATE),action(NO_ACTION),token(NULL),actionInfo(NULL),needPushBack(false){}

State::State(int nextStateNum):nextStateNum(nextStateNum),action(NO_ACTION),token(NULL),actionInfo(NULL),needPushBack(false){}

State::State(bool needPushBack,int type,int subtype,string lexeme):nextStateNum(0),needPushBack(needPushBack),token(new tokenClass(type,subtype,lexeme)),action(ACCEPT),actionInfo(NULL){}

State::State(string errorMessage):nextStateNum(0),action(ERROR),needPushBack(false),token(NULL),actionInfo(new string(errorMessage)){}

State::State(int nextStateNum,Action sideAction):nextStateNum(nextStateNum),action(sideAction),needPushBack(false),token(NULL),actionInfo(NULL){}

State::State(int nextStateNum,bool needPushBack):nextStateNum(nextStateNum),action(NO_ACTION),needPushBack(needPushBack),token(NULL),actionInfo(NULL){}

std::ostream& operator<<(std::ostream &strm, const State &s) {
	if(s.token != NULL)
	{
		return  strm <<"{"<< s.nextStateNum <<"}"<<tokenClass::tokenIntToString(s.token->type)<<" "<<tokenClass::tokenIntToString(s.token->subtype)<<" "<<s.token->lexeme;
	}
	return strm <<"{"<< s.nextStateNum <<"}"<<"NULL";
}


scannerClass::scannerClass()
{
	buildStateMatrix();
}

tokenClass scannerClass::getToken()
{
	int currentStateNum = 0;
	int c;
	string currentLexeme;
	do
	{
		//First clear the buffer if it is a new state.
		if(currentStateNum == 0)
			currentLexeme = "";

		c = (int)fileManager.getNextChar();
		if(c == EOF)//Because EOF(-1) is not a valid index, we need to change it before using it in the matrix.
			c = EOF_INDEX;

		if(c >= MAX_CHAR)//If the char read is not within the range.
		{
			errorAndExit(("Illegal symbol: "+c));
			return tokenClass(EOF_T,NONE_ST,"EOF");
		}
		State s = stateMatrix[currentStateNum][c];//Get the next state.
		if(s.nextStateNum == INVALID_STATE)//If state is invalid, e.g. the char sequence is not recognized.
		{
			errorAndExit("Invalid char sequence: "+(currentLexeme+(char)c));
			return tokenClass(EOF_T,NONE_ST,"EOF");
		}
		if(s.nextStateNum == EOF_INDEX)//If reached EOF, in this case no action needs to be taken.
			return tokenClass(EOF_T,NONE_ST,"EOF");

		switch(s.action)
		{
		case NO_ACTION:
			currentLexeme += c;//Store the current char.
			break;
		case ACCEPT:
			{
				if(!s.needPushBack)
					currentLexeme += c;
				else
					fileManager.pushBack();

				int type = s.token->type;

				if(type == INTEGER_T)//Check for interger length
					if(currentLexeme.length() > 4)
					{
						errorAndExit("Integer can have at most four digits: "+currentLexeme);
						return tokenClass(EOF_T,NONE_ST,"EOF");
					}

				if(type == STRING_T)//Check for string length
					if(currentLexeme.length() > 52)
					{
						errorAndExit("String can have at most fifty characters: "+currentLexeme);
						return tokenClass(EOF_T,NONE_ST,"EOF");
					}

				if(type == IDENTIFIER_T)//Check for identifier length
				{
					if(currentLexeme.length() > 12)
					{
						errorAndExit("Identifier can have at most twelve characters: "+currentLexeme);
						return tokenClass(EOF_T,NONE_ST,"EOF");
					}

					//Here we check for the keywords
					const char* cString = currentLexeme.c_str();
					if(_strcmpi("or",cString)==0)
						return tokenClass(ADDOP_T,OR_ST,currentLexeme);
					if(_strcmpi("and",cString)==0)
						return tokenClass(MULOP_T,AND_ST,currentLexeme);
					if(_strcmpi("div",cString)==0)
						return tokenClass(MULOP_T,DIV_ST,currentLexeme);
					if(_strcmpi("mod",cString)==0)
						return tokenClass(MULOP_T,MOD_ST,currentLexeme);

					//Here list all keywords in an array. Their sequence should not be changed.
					char* keywords[16]={"program","function","begin","end","if","then","else","while","do","cout","cin","endl","int","boolean","true","false"};
					for(int i = 0;i<16;i++)
						if(_strcmpi(keywords[i],cString)==0)
							return tokenClass(KEYWORD_BASE+i*10,NONE_ST,currentLexeme);
				}

				return tokenClass(s.token->type,s.token->subtype,currentLexeme);
			}
		case WARNING:
			cout<<"warning"<<endl;//Currently not used because no warning state is checked by the state matrix.
			break;
		case ERROR:
			{
				if(s.actionInfo != NULL)
					errorAndExit(*s.actionInfo);
				else //This else is only here as a double check. Normally an error message should be included.
					errorAndExit("Error");
			}
		case CLEAR_BUFFER:
			currentLexeme = "";//Clear buffer, for comments only.
			break;
		case CHECK_COMPILER_DIRECTIVE:
			currentLexeme+=c;
			if(currentLexeme != "{$p+}")
				fileManager.setPrintStatus(true);
			else if (currentLexeme != "{$p-}")
				fileManager.setPrintStatus(false);
			else
				cout<<"Warning, compiler directive "+currentLexeme+" is undefined."<<endl;
			break;
		}

		currentStateNum = s.nextStateNum;
	}
	while(true);

	//This should be unreachable
	return tokenClass(EMPTY_T,EMPTY_ST,EMPTY_LEXEME);
}

void scannerClass::buildStateMatrix()
{
	int nonFinalStateNum = 0;

	for(int i = 0;i<MAX_STATE;++i)
		for(int j = 0;j<MAX_CHAR;++j)
			stateMatrix[i][j] = State();

	//Read carriage return and whitespace at state 0 lead to 0
	State zeroState(0);
	char whitespaces[6] = {' ','\t','\v','\f','\r','\n'};
	for(int i = 0;i<6;++i)
		stateMatrix[0][whitespaces[i]]=zeroState;
	stateMatrix[0][EOF_INDEX] = State(EOF_INDEX);//End of file indicator

	//Read *-+.,()~;& goes to the respective final state with action ACCEPT
	stateMatrix[0]['*'] = State(false,MULOP_T,MULTIPLY_ST,"*");
	stateMatrix[0]['-'] = State(false,ADDOP_T,SUBSTRACT_ST,"-");
	stateMatrix[0]['+'] = State(false,ADDOP_T,ADD_ST,"+");
	stateMatrix[0]['.'] = State(false,DOT_T,NONE_ST,".");
	stateMatrix[0][','] = State(false,COMMA_T,NONE_ST,",");
	stateMatrix[0]['('] = State(false,LEFTPAREN_T,NONE_ST,"(");
	stateMatrix[0][')'] = State(false,RIGHTPAREN_T,NONE_ST,")");
	stateMatrix[0]['~'] = State(false,TILDE_T,NONE_ST,"~");
	stateMatrix[0][';'] = State(false,SEMICOLON_T,NONE_ST,";");
	stateMatrix[0]['&'] = State(false,AMPERSAND_T,NONE_ST,"&");

	//Read "="
	int firstEqualStateNum = ++nonFinalStateNum;//State with number zero is reserved for the starting state
	stateMatrix[0]['='] = State(firstEqualStateNum);//Read first equal
	for(int i = 0;i<MAX_CHAR;++i)
	{
		if(i == '=')
			stateMatrix[firstEqualStateNum]['=']=State(false,RELOP_T,EQUAL_ST,"==");
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
			stateMatrix[firstGreaterThanStateNum]['>'] = State(false,BITWISE_T,BITRIGHT_ST,">>");
		else if (i == '=')
			stateMatrix[firstGreaterThanStateNum]['='] = State(false,RELOP_T,GREATEROREQUAL_ST,">=");
		else
			stateMatrix[firstGreaterThanStateNum][i] = State(true,RELOP_T,GREATER_ST,">");
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
		else if(i=='\r'||i=='\n')
			stateMatrix[leftQuoteStateNum][i] = State("String cannot be split across lines.");
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
		if(('A'<=i&&i<='Z')||('a'<=i&&i<='z')||('0'<=i&&i<='9')||(i=='_'))
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

void scannerClass::buildStateMatrixCompact()
{
	int nonFinalStateNum = 0;

	for(int i = 0;i<MAX_STATE;++i)
		for(int j = 0;j<MAX_CHAR;++j)
			stateMatrix[i][j] = State();

	//Read carriage return and whitespace at state 0 lead to 0
	State zeroState(0);
	char whitespaces[6] = {' ','\t','\v','\f','\r','\n'};
	for(int i = 0;i<6;++i)
		stateMatrix[0][whitespaces[i]]=zeroState;
	stateMatrix[0][EOF_INDEX] = State(EOF_INDEX);//End of file indicator

	//Read *-+.,()~;& goes to the respective final state with action ACCEPT
	stateMatrix[0]['*'] = State(false,MULOP_T,MULTIPLY_ST,"*");
	stateMatrix[0]['-'] = State(false,ADDOP_T,SUBSTRACT_ST,"-");
	stateMatrix[0]['+'] = State(false,ADDOP_T,ADD_ST,"+");
	stateMatrix[0]['.'] = State(false,DOT_T,NONE_ST,".");
	stateMatrix[0][','] = State(false,COMMA_T,NONE_ST,",");
	stateMatrix[0]['('] = State(false,LEFTPAREN_T,NONE_ST,"(");
	stateMatrix[0][')'] = State(false,RIGHTPAREN_T,NONE_ST,")");
	stateMatrix[0]['~'] = State(false,TILDE_T,NONE_ST,"~");
	stateMatrix[0][';'] = State(false,SEMICOLON_T,NONE_ST,";");
	stateMatrix[0]['&'] = State(false,AMPERSAND_T,NONE_ST,"&");

	//Read "="
	int firstEqualStateNum = ++nonFinalStateNum;//State with number zero is reserved for the starting state
	stateMatrix[0]['='] = State(firstEqualStateNum);//Read first equal
	//Read "<"
	int firstLessThanStateNum = ++nonFinalStateNum;
	stateMatrix[0]['<'] = State(firstLessThanStateNum);
	//Read ">"
	int firstGreaterThanStateNum = ++nonFinalStateNum;
	stateMatrix[0]['>']=State(firstGreaterThanStateNum);
	//Read digit
	int firstDigitStateNum = ++nonFinalStateNum;
	string digits = "0123456789";
	for(int i = 0;i<digits.length();++i)
	{
		stateMatrix[0][digits[i]] = State(firstDigitStateNum);
	}
	//Read string
	int leftQuoteStateNum = ++nonFinalStateNum;
	stateMatrix[0]['\''] = State(leftQuoteStateNum);
	//Read double slash started comment
	int firstSlashStateNum = ++nonFinalStateNum;
	int secondSlashStateNum = ++nonFinalStateNum;
	stateMatrix[0]['/']=State(firstSlashStateNum);
	//Read identifier
	int firstLetterStateNum = ++nonFinalStateNum;
		string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	for(int i = 0;i<letters.length();++i)
		stateMatrix[0][letters[i]]=State(firstLetterStateNum);
	//Read comment enclosed in brace
	int leftBraceStateNum = ++nonFinalStateNum;
	int secondDollarStateNum = ++nonFinalStateNum;
	int thirdLetterStateNum = ++nonFinalStateNum;
	int fourthAddMinusStateNum = ++nonFinalStateNum;
	int bracedCommentStateNum = ++nonFinalStateNum;
	stateMatrix[0]['{'] = State(leftBraceStateNum);

	for(int i = 0;i<MAX_CHAR;++i)
	{
		//"="
		if(i == '=')
			stateMatrix[firstEqualStateNum]['=']=State(false,RELOP_T,EQUAL_ST,"==");
		else
			stateMatrix[firstEqualStateNum][i] = State(true,ASSIGNMENT_T,NONE_ST,"=");
		//"<"
		if(i == '<')
			stateMatrix[firstLessThanStateNum]['<'] = State(false,BITWISE_T,BITLEFT_ST,"<<");
		else if(i=='>')
			stateMatrix[firstLessThanStateNum]['>'] = State(false,RELOP_T,UNEQUAL_ST,"<>");
		else if (i == '=')
			stateMatrix[firstLessThanStateNum]['='] = State(false,RELOP_T,LESSOREQUAL_ST,"<=");
		else
			stateMatrix[firstLessThanStateNum][i] = State(true,RELOP_T,LESS_ST,"<");
		//">"
		if(i == '>')
			stateMatrix[firstGreaterThanStateNum]['>'] = State(false,BITWISE_T,BITRIGHT_ST,">>");
		else if (i == '=')
			stateMatrix[firstGreaterThanStateNum]['='] = State(false,RELOP_T,GREATEROREQUAL_ST,">=");
		else
			stateMatrix[firstGreaterThanStateNum][i] = State(true,RELOP_T,GREATER_ST,">");
		if(i<= '9' && i >='0')
			stateMatrix[firstDigitStateNum][i] = State(firstDigitStateNum);
		else
			stateMatrix[firstDigitStateNum][i] = State(true,INTEGER_T,NONE_ST,"int_lexeme");
		//string
		if(i == '\'')
			stateMatrix[leftQuoteStateNum][i] = State(false,STRING_T,NONE_ST,"string_lexeme");
		else if(i=='\r'||i=='\n')
			stateMatrix[leftQuoteStateNum][i] = State("String cannot be split across lines.");
		else if (i == EOF_INDEX)
			stateMatrix[leftQuoteStateNum][i] = State("Single quote expected at the end of string.");
		else
			stateMatrix[leftQuoteStateNum][i]=State(leftQuoteStateNum);
		//double slash started comment
		if(i == '/')
			stateMatrix[firstSlashStateNum][i] = State(secondSlashStateNum);
		else 
			stateMatrix[firstSlashStateNum][i] = State("Comment should be started by double slashes.");
		if(i == '\r'||i == '\n')
			stateMatrix[secondSlashStateNum][i] = State(0);
		else if(i == EOF_INDEX)
			stateMatrix[secondSlashStateNum][i] = State(0,true);
		else
			stateMatrix[secondSlashStateNum][i] = State(secondSlashStateNum,CLEAR_BUFFER);
		//Identifier
		if(('A'<=i&&i<='Z')||('a'<=i&&i<='z')||('0'<=i&&i<='9')||(i=='_'))
			stateMatrix[firstLetterStateNum][i] = State(firstLetterStateNum);
		else 
			stateMatrix[firstLetterStateNum][i] = State(true,IDENTIFIER_T,NONE_ST,"identifier_lexeme");
		//Braced comment
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


void scannerClass::printStateMatrix()
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

void scannerClass::close()
{
	fileManager.closeSourceProgram();
	//Clear the memory used by the state matrix
	for(int i = 0;i<MAX_STATE;i++)
		for(int j = 0;j<MAX_CHAR;j++)
		{
			delete stateMatrix[i][j].actionInfo;
			delete stateMatrix[i][j].token;
		}
}

void scannerClass::errorAndExit(string message)
{
	cout<<"Compilation error at line "<<":"<<endl;
	fileManager.printCurrentLine();
	cout<<message<<endl;
	close();
	cin.get();
	exit(0);
}
