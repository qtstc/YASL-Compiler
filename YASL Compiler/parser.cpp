//Tao Qian
//This file contains the method definition for parserClass.

//The class header is found in parser.h

#include "stdafx.h"
#include "parser.h"

void parserClass::parseExpr()
{
	pStackClass stack;
	stack.push(tokenClass(SEMICOLON_T,NONE_ST,EMPTY_LEXEME));
	tokenClass t = scanner.getToken();
	while(true)
	{
		tokenClass topTerm = stack.getTopMostTerminal();
		if(topTerm.type == t.type && topTerm.type == SEMICOLON_T)
			return;
		Precedence p = prec(topTerm,t);
		if(p == LESS_PRECEDENCE || p == EQUAL_PRECEDENCE)//Shift
		{
			stack.push(t);
			t = scanner.getToken();
		}
		else if(p == GREATER_PRECEDENCE)
		{
			std::vector<tokenClass> tokens;
			tokenClass topMost;
			stack.lastTerminalPopped = tokenClass(EMPTY_T,EMPTY_ST,EMPTY_LEXEME);
			do
			{
				tokens.push_back(stack.pop());
			}
			while(stack.lastTerminalPopped.type == EMPTY_T
				||(!stack.terminalOnTop())
				||prec(stack.getTopMostTerminal(),stack.lastTerminalPopped) != LESS_PRECEDENCE);
			if(isValidRHS(tokens))
			{
				for(int i = 0;i<tokens.size();++i)
				{
					cout<<tokenClass::tokenIntToString(tokens[tokens.size()-1-i].type)<<" ";
				}
				cout<<"->E"<<endl;
				stack.push(tokenClass(E_T,NONE_ST,EMPTY_LEXEME));
			}
			else
			{
				stack.clear();
				errorAndExit("Invalid right hand side.");
			}
		}
		else
		{
			stack.clear();
			errorAndExit("Invalid expression sequnce");
		}
	}

}
parserClass::parserClass()
{
	buildPrecedenceTable();
	printPrecedenceTable();
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
	default:
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

	tokenClass last = tokens[0];
	tokenClass middle = tokens[1];
	tokenClass first = tokens[2];
	//First check the case of (E)
	if(first.type == LEFTPAREN_T && last.type == RIGHTPAREN_T)
		return isValidRHS(std::vector<tokenClass>(1,middle));
	//Then check the case of E X E, where X is a YASL operator
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

}
void parserClass::errorAndExit(string message)
{
	cout<<"Invalid expression at line "<<":"<<endl;
	scanner.printCurrentLine();
	cout<<message<<endl;
	scanner.close();
	cin.get();
	exit(0);
}