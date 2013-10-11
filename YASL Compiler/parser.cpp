//Tao Qian
//This file contains the method definition for parserClass.

//The class header is found in parser.h

#include "stdafx.h"
#include "parser.h"

parserClass::parserClass()
{
	buildPrecedenceTable();
	//printPrecedenceTable();
}


void parserClass::parseProgram()
{
	t = scanner.getToken();
	parseStatement();
	cout<< "YASLC-TQ has just compiled "<<scanner.numLinesProcessed()<<" lines successfully."<<endl;
	scanner.close();
}

void parserClass::parseStatement()
{
	switch(t.type)
	{
	case WHILE_T:
		t = scanner.getToken();
		parseExpr();
		checkTokenAndGetNext(t,tokenClass(DO_T,NONE_ST,"do"));
		parseStatement();
		break;
	case IDENTIFIER_T:
		t=scanner.getToken();
		checkTokenAndGetNext(t,tokenClass(ASSIGNMENT_T,NONE_ST,"="));
		//Parsed "="
		parseExpr();
		checkTokenAndGetNext(t,tokenClass(SEMICOLON_T,NONE_ST,";"));
		break;
	default:
		vector<string> found;
		found.push_back("while");
		found.push_back("identifier");
		recurDescentErrorAndExit(t.lexeme,found);
	}
}

void parserClass::parseExpr()
{
	pStackClass stack;//Stack used in the algorithm.
	stack.push(tokenClass(SEMICOLON_T,NONE_ST,EMPTY_LEXEME));//First push a semicolon onto the stack.
	if(isEndOfExpression(t))
		errorAndExit("Empty expression");
	while(true)
	{
		tokenClass topTerm = stack.getTopMostTerminal();
		//Termination condition.
		//It should be met if the expression is valid and terminate with semicolon
		if(isEndOfExpression(t) && isEndOfExpression(topTerm))
			return;
		Precedence p = prec(topTerm,t);//Store the precedence because it’s checked multiple times.
		if(p == LESS_PRECEDENCE || p == EQUAL_PRECEDENCE)//Shift
		{
			stack.push(t);
			t = scanner.getToken();
		}
		else if(p == GREATER_PRECEDENCE)//Reduce
		{
			std::vector<tokenClass> tokens;
			stack.lastTerminalPopped = tokenClass(EMPTY_T,EMPTY_ST,EMPTY_LEXEME);//Reset the lastTerminalPopped
			do
			{
				tokenClass popped = stack.pop();
				if(popped.type == EMPTY_T)//If the stack is empty. This should not happen in normal operation.
					errorAndExit("Empty stack.");
				tokens.push_back(popped);
			}
			while(stack.lastTerminalPopped.type == EMPTY_T
				||(!stack.terminalOnTop())
				||prec(stack.getTopMostTerminal(),stack.lastTerminalPopped) != LESS_PRECEDENCE);
			if(isValidRHS(tokens))
			{
				if(scanner.expressionDebugging)//Print debug message if expression debugging is turned on.
				{
					cout<<"E -> ";
					for(int i = 0;i<tokens.size();++i)
						cout<<tokens[tokens.size()-1-i].lexeme<<" ";
					cout<<endl;
				}
				//Replace the expression in the array with a E if the expression is valid.
				stack.push(tokenClass(E_T,NONE_ST,"E"));
			}
			else//If expression is invalid, throw error.
				errorAndExit("Invalid right hand side.");
		}
		else//If precedence is not valid, throw error.
			errorAndExit("Invalid expression");
	}
}

void parserClass::buildPrecedenceTable()
{
	//First fill each cell with error precedence
	for(int i = 0;i<PRECEDENCE_TABLE_DIMENSION;++i)
		for(int j = 0;j<PRECEDENCE_TABLE_DIMENSION;++j)
			precedenceTable[i][j] = ERROR_PRECEDENCE;

	int leftAssoOp[3] = {MULOP_T,ADDOP_T,RELOP_T}; 
	int identifiers[4] = {TRUE_T,FALSE_T,IDENTIFIER_T,INTEGER_T};

	//Left associate operators has GREATER_PRECEDENCE
	for(int i = 0;i<3;++i)
		for(int j = 0;j<3;++j)
			setPre(leftAssoOp[i],leftAssoOp[j],GREATER_PRECEDENCE);

	//ADDOP < MULOP
	setPre(ADDOP_T,MULOP_T,LESS_PRECEDENCE);

	//RELOP < ADDOP, RELOP < MULOP
	setPre(RELOP_T,ADDOP_T,LESS_PRECEDENCE);
	setPre(RELOP_T,MULOP_T,LESS_PRECEDENCE);

	//Operator precedence
	for(int i = 0;i<3;++i)
	{
		//op < identifier, identifier > op
		for(int j = 0;j<4;++j)
		{
			setPre(leftAssoOp[i],identifiers[j],LESS_PRECEDENCE);
			setPre(identifiers[j],leftAssoOp[i],GREATER_PRECEDENCE);
		}
		//op < (, ( < op
		setPre(leftAssoOp[i],LEFTPAREN_T,LESS_PRECEDENCE);
		setPre(LEFTPAREN_T,leftAssoOp[i],LESS_PRECEDENCE);
		//op > ), a > op
		setPre(leftAssoOp[i],RIGHTPAREN_T,GREATER_PRECEDENCE);
		setPre(RIGHTPAREN_T,leftAssoOp[i],GREATER_PRECEDENCE);
		//op > ;, ; < op
		setPre(leftAssoOp[i],SEMICOLON_T,GREATER_PRECEDENCE);
		setPre(SEMICOLON_T,leftAssoOp[i],LESS_PRECEDENCE);
	}

	// ( = )
	setPre(LEFTPAREN_T,RIGHTPAREN_T,EQUAL_PRECEDENCE);
	// ; < (
	setPre(SEMICOLON_T,LEFTPAREN_T,LESS_PRECEDENCE);
	// ( < (
	setPre(LEFTPAREN_T,LEFTPAREN_T,LESS_PRECEDENCE);
	// ) > ;
	setPre(RIGHTPAREN_T,SEMICOLON_T,GREATER_PRECEDENCE);
	// ) > )
	setPre(RIGHTPAREN_T,RIGHTPAREN_T,GREATER_PRECEDENCE);

	for(int i = 0;i<4;++i)
	{
		//identifier > i , ; < identifier
		setPre(identifiers[i],SEMICOLON_T,GREATER_PRECEDENCE);
		setPre(SEMICOLON_T,identifiers[i],LESS_PRECEDENCE);
		//( < identifier
		setPre(LEFTPAREN_T,identifiers[i],LESS_PRECEDENCE);
		// identifier > )
		setPre(identifiers[i],RIGHTPAREN_T,GREATER_PRECEDENCE);
	}
}

void parserClass::printPrecedenceTable()
{
	ofstream myfile;
	myfile.open ("precedenceTable.csv");

	string names[PRECEDENCE_TABLE_DIMENSION] = {"rel","add","mul","(",")","id","ic","true","false",";","other"};
	myfile<<",";
	for(int i = 0;i<PRECEDENCE_TABLE_DIMENSION;++i)
		myfile<<names[i]<<",";
	myfile<<"\n";
	for(int i = 0;i<PRECEDENCE_TABLE_DIMENSION;++i)
	{
		myfile<<names[i]<<",";
		for(int j = 0;j<PRECEDENCE_TABLE_DIMENSION;++j)
		{
			if(precedenceTable[i][j] == GREATER_PRECEDENCE)
				myfile<<"G";
			else if(precedenceTable[i][j] == LESS_PRECEDENCE)
				myfile<<"L";
			else if(precedenceTable[i][j] == ERROR_PRECEDENCE)
				myfile<<"E";
			else myfile<< "EQ";
			myfile<<",";
		}
		myfile<<"\n";
	}
	myfile.close();
}

void parserClass::setPre(int col,int row, Precedence pre)
{
	precedenceTable[typeToTableIndex(col)][typeToTableIndex(row)] = pre;
}

int parserClass::typeToTableIndex(int type)
{
	if(isEndOfExpression(type))
		return 9;

	switch(type)
	{
	case RELOP_T:
		return 0;
	case ADDOP_T:
		return 1;
	case MULOP_T:
		return 2;
	case LEFTPAREN_T:
		return 3;
	case RIGHTPAREN_T:
		return 4;
	case IDENTIFIER_T:
		return 5;
	case INTEGER_T:
		return 6;
	case TRUE_T:
		return 7;
	case FALSE_T:
		return 8;
	case SEMICOLON_T:
		return 9;
	default://Everything else is considered other.
		return 10;
	}
}
Precedence parserClass::prec(int firstType,int secondType)
{
	return precedenceTable[typeToTableIndex(firstType)][typeToTableIndex(secondType)];
}

Precedence parserClass::prec(tokenClass firstToken,tokenClass secondToken)
{
	return prec(firstToken.type,secondToken.type);
}
bool parserClass::isValidRHS(std::vector<tokenClass> tokens)
{
	//Base case of a single terminal
	if(tokens.size() == 1)
	{
		tokenClass token = tokens.back();
		if(token.type == INTEGER_T
			||token.type == IDENTIFIER_T
			||token.type == TRUE_T
			||token.type == FALSE_T
			||token.type == E_T)
			return true;
		return false;
	}
	//All the other cases have three tokens
	if(tokens.size() != 3)
		return false;

	//Reversed because of the stack.
	tokenClass last = tokens[0];
	tokenClass middle = tokens[1];
	tokenClass first = tokens[2];
	//First check the case of (E)
	if(first.type == LEFTPAREN_T && last.type == RIGHTPAREN_T)
		return isValidRHS(std::vector<tokenClass>(1,middle));
	//Then check the case of E X E, where X is a YASL operator
	//If the expression at both ends of the expression are valid.
	if(isValidRHS(std::vector<tokenClass>(1,first))&&isValidRHS(std::vector<tokenClass>(1,last)))
	{
		//Check +
		if(middle.type == ADDOP_T && middle.subtype == ADD_ST)
			return true;
		//Check *
		if(middle.type == MULOP_T && middle.subtype == MULTIPLY_ST)
			return true;
		//Check -
		if(middle.type == ADDOP_T && middle.subtype == SUBSTRACT_ST)
			return true;
		//Check div
		if(middle.type == MULOP_T && middle.subtype == DIV_ST)
			return true;
		//Check mod
		if(middle.type == MULOP_T && middle.subtype == MOD_ST)
			return true;
		//Check or
		if(middle.type == ADDOP_T && middle.subtype == OR_ST)
			return true;
		//Check and
		if(middle.type == MULOP_T && middle.subtype == AND_ST)
			return true;
		//Check ==
		if(middle.type == RELOP_T && middle.subtype == EQUAL_ST)
			return true;
		//Check <
		if(middle.type == RELOP_T && middle.subtype == LESS_ST)
			return true;
		//Check <=
		if(middle.type == RELOP_T && middle.subtype == LESSOREQUAL_ST)
			return true;
		//Check >
		if(middle.type == RELOP_T && middle.subtype == GREATER_ST)
			return true;
		//Check >=
		if(middle.type == RELOP_T && middle.subtype == GREATEROREQUAL_ST)
			return true;
		//Check <>
		if(middle.type == RELOP_T && middle.subtype == UNEQUAL_ST)
			return true;
		return false;
	}
	return false;
}

void parserClass::recurDescentErrorAndExit(string found, vector<string> expected)
{
	string s = "Found <"+found+"> when expecting one of <";
	for(int i = 0;i<expected.size()-1;++i)
	{
		s += expected[i]+",";
	}
	s += expected[expected.size()-1]+">";
	errorAndExit(s);
}

void parserClass::errorAndExit(string message)
{
	cout<<"Syntax Error at line ";
	scanner.printCurrentLine();
	cout<<message<<endl;
	scanner.close();
	cin.get();
	exit(0);
}

bool parserClass::isEndOfExpression(tokenClass token)
{
	if(isEndOfExpression(token.type))
		return true;
	return false;
}
bool parserClass::isEndOfExpression(int type)
{
	if(type == DO_T || type == SEMICOLON_T)
		return true;
	return false;
}
void parserClass::checkTokenAndGetNext(tokenClass token, tokenClass expected)
{
	if(token.type != expected.type)
		recurDescentErrorAndExit(token.lexeme,vector<string>(1,expected.lexeme));
	t = scanner.getToken();
}