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
	//Open a new file for writing the generated code.
	outfile.open("..\\Pali\\out.pal",ios::out);
	//outfile.open("out.pal",ios::out);

	t = scanner.getToken();
	checkTokenAndGetNext(t,tokenClass(PROGRAM_T,NONE_ST,"program"));

	printBranch("$main");
	//Store the bottom of the stack to R0.
	printInstruction(PAL_MOVW,NULL,PAL_SP,NULL,PAL_R0);

	scanner.symbolTable.tableAddLevel(t.lexeme);
	checkTokenAndGetNext(t,IDENTIFIER_TOKEN);
	checkTokenAndGetNext(t,SEMICOLON_TOKEN);
	parseBlock();
	scanner.symbolTable.tableDelLevel();
	printInstruction(PAL_INB,NULL,toPALDirectAddressing(PAL_SP));
	outfile<<"end";
	checkTokenAndGetNext(t,tokenClass(DOT_T,NONE_ST,"."));
	cout<< "YASLC-TQ has just compiled "<<scanner.numLinesProcessed()<<" lines successfully."<<endl;
	scanner.close();
	outfile.close();
}


void parserClass::parseVarDecs()
{
	SymbolType type = tryParseType();
	if(type == INVALID_TYPE)
		return;
	parseIdentList(type);
	checkTokenAndGetNext(t,SEMICOLON_TOKEN);
	parseVarDecs();
}

SymbolType parserClass::tryParseType()
{
	if(t.type == INT_T)
	{
		t = scanner.getToken();
		return INT_TYPE;
	}
	else if (t.type == BOOLEAN_T)
	{
		t = scanner.getToken();
		return BOOLEAN_TYPE;
	}
	return INVALID_TYPE;
}

void parserClass::parseIdentList(SymbolType type)
{
	SymbolNode* symbol = new SymbolNode(t.lexeme,VAR_ID,type);
	//Check duplicate and throw an error if duplicate is found.
	if(!scanner.symbolTable.tableAddEntry(symbol))
		errorAndExit("Duplicate identifier ["+t.lexeme+"] in the same scope.");
	checkTokenAndGetNext(t,IDENTIFIER_TOKEN);
	parseIdentTail(type);
}

void parserClass::parseIdentTail(SymbolType type)
{
	if(t.type == COMMA_T)
	{
		t = scanner.getToken();
		parseIdentList(type);
	}
}

void parserClass::parseFuncDecs()
{
	if(t.type != FUNCTION_T)
		return;
	t = scanner.getToken();
	//First add the function to the table in cuurent scope
	SymbolNode* funcSymbol = new SymbolNode(t.lexeme,FUNC_ID,FUNC_ID_TYPE);
	if(!scanner.symbolTable.tableAddEntry(funcSymbol))
		errorAndExit("Duplicate identifier ["+t.lexeme+"] in the same scope.");
	string palLabel = getNextTempName();
	funcSymbol->PALLabel = palLabel;
	//Start of the function call in PAL
	printBranch(palLabel);
	//Then create a new level of table for the function
	scanner.symbolTable.tableAddLevel(t.lexeme);
	checkTokenAndGetNext(t,IDENTIFIER_TOKEN);
	parseFuncDecTail();
	checkTokenAndGetNext(t,SEMICOLON_TOKEN);
	parseBlock();
	scanner.symbolTable.tableDelLevel();
	checkTokenAndGetNext(t,SEMICOLON_TOKEN);
	//end of the function call in PAL
	printInstruction(PAL_MOVW,NULL,PAL_FP,NULL,PAL_SP);
	printInstruction(PAL_RET,NULL,"");
	parseFuncDecs();
}

void parserClass::parseFuncDecTail()
{
	if(t.type != LEFTPAREN_T)
		return;
	t = scanner.getToken();
	parseParamList();
	checkTokenAndGetNext(t,tokenClass(RIGHTPAREN_T,NONE_ST,")"));
}
void parserClass::parseParamList()
{
	SymbolType type = tryParseType();
	if(type != INVALID_TYPE)
	{
		parseTypeTail(type);
		return;
	}
	vector<string> expected;
	expected.push_back("int");
	expected.push_back("boolean");
	recurDescentErrorAndExit(t.lexeme,expected);
}
void parserClass::parseTypeTail(SymbolType type)
{
	SymbolKind kind = VALUE_PARAM;
	if(t.type == AMPERSAND_T)
	{
		t = scanner.getToken();
		kind = REF_PARAM;
	}
	if(!scanner.symbolTable.addFunctionParameter(new SymbolNode(t.lexeme,kind,type)))
		errorAndExit("Duplicate identifier ["+t.lexeme+"] in the same scope.");
	if(!scanner.symbolTable.tableAddEntry(new SymbolNode(t.lexeme,kind,type)))
		errorAndExit("Duplicate identifier ["+t.lexeme+"] in the same scope.");
	checkTokenAndGetNext(t,IDENTIFIER_TOKEN);
	parseFuncIdentTail();//NOTE:it's func ident tail here!
}
void parserClass::parseFuncIdentTail()
{
	if(t.type == COMMA_T)
	{
		t= scanner.getToken();
		parseParamList();
	}
}
void parserClass::parseBlock()
{
	parseVarDecs();
	//Move SP up to give space for the global variables
	int stackDiff = PAL_WORD_SIZE*scanner.symbolTable.top->nextOffset;
	printInstruction(PAL_ADDW,NULL,toPALLiteral(stackDiff),NULL,PAL_SP);
	//Get the name of the block and jump there
	string blockName = getNextTempName();
	printInstruction(PAL_JMP,NULL,blockName);
	parseFuncDecs();
	printBranch(blockName);
	parseProgBody();
}

void parserClass::parseProgBody()
{
	checkTokenAndGetNext(t,tokenClass(BEGIN_T,NONE_ST,"begin"));
	parseFollowBegin();
}

void parserClass::parseStatement()
{
	switch(t.type)
	{
	case WHILE_T:
		{
			string topBranch = getNextTempName();
			string afterBranch = getNextTempName();
			printBranch(topBranch);//Start of top branch
			t = scanner.getToken();
			
			SymbolType resultType = parseExpr();
			if(resultType != BOOLEAN_TYPE)
				errorAndExit("The condition statement of while loop must evaluate to a boolean.");
			//Compare 0 with with the result of the condition statement of while loop.
			printInstruction(PAL_CMPW,NULL,toPALLiteral(0),NULL,toPALDirectAddressing(PAL_R1));
			//Clear the temp variable.
			printInstruction(PAL_MOVW,NULL,PAL_R1,NULL,PAL_SP);
			//Print after branch
			printInstruction(PAL_BEQ,NULL,afterBranch);
			checkTokenAndGetNext(t,tokenClass(DO_T,NONE_ST,"do"));
			parseStatement();
			//jump statement
			printInstruction(PAL_JMP,NULL,topBranch);
			//print after label
			printBranch(afterBranch);//end of while
		}
		break;
	case IF_T:
		{
			t = scanner.getToken();
			string ifBranch = getNextTempName();
			string elseBranch = getNextTempName();
			SymbolType resultType = parseExpr();
			if(resultType != BOOLEAN_TYPE)
				errorAndExit("The condition statement of if loop must evaluate to a boolean.");
			//Compare 0 with with the result of the condition statement of if.
			printInstruction(PAL_CMPW,NULL,toPALLiteral(0),NULL,toPALDirectAddressing(PAL_R1));
			//Clear the temp variable.
			printInstruction(PAL_MOVW,NULL,PAL_R1,NULL,PAL_SP);
			//Print skip branch
			printInstruction(PAL_BEQ,NULL,ifBranch);
			checkTokenAndGetNext(t,tokenClass(THEN_T,NONE_ST,"then"));
			parseStatement();
			//jump statement
			printInstruction(PAL_JMP,NULL,elseBranch);
			printBranch(ifBranch);
			//Dummy statement required because if there is no else statement,
			//two branch names will be on the same line, which is invalid.
			printInstruction(PAL_MOVW,NULL,PAL_R0,NULL,PAL_R0);
			parseFollowIf();
			printBranch(elseBranch);
		}
		break;
	case IDENTIFIER_T:
		{
			SymbolNode* id = checkId(t.lexeme);
			t=scanner.getToken();
			parseFollowID(id);
		}
		break;
	case BEGIN_T:
		{
			t=scanner.getToken();
			parseFollowBegin();
		}
		break;
	case CIN_T:
		{
			t= scanner.getToken();
			parseFollowCin(true);
		}
		break;
	case COUT_T:
		{
			t= scanner.getToken();
			checkTokenAndGetNext(t,tokenClass(BITLEFT_T,NONE_ST,"<<"));
			parseFollowCout();
			parseCoutTail();
		}
		break;
	default:
		vector<string> expected;
		expected.push_back("while");
		expected.push_back("if");
		expected.push_back("identifier");
		expected.push_back("begin");
		expected.push_back("cin");
		expected.push_back("cout");
		recurDescentErrorAndExit(t.lexeme,expected);
	}
}

void parserClass::parseFollowBegin()
{
	if(t.type == END_T)
	{
		t = scanner.getToken();
		return;
	}
	parseStatement();
	parseStatementTail();
	checkTokenAndGetNext(t,tokenClass(END_T,NONE_ST,"end"));
}

void parserClass::parseStatementTail()
{
	if(t.type == SEMICOLON_T)
	{
		t = scanner.getToken();
		parseStatement();
		parseStatementTail();
	}
}

void parserClass::parseFollowCin(bool onlyParam)
{
	if(t.type == BITRIGHT_T)
	{
		t = scanner.getToken();
		SymbolNode* id = checkId(t.lexeme);
		if(id->type != INT_TYPE)
			errorAndExit("cin can only take integer variable, "+string(id->lexeme)+" is "+string(symbolTypeStrings[id->type]));
		printInstruction(PAL_INW,id,"");
		checkTokenAndGetNext(t,IDENTIFIER_TOKEN);
		parseFollowCin(false);
	}
	else if(onlyParam)
	{
		printInstruction(PAL_INB,NULL,toPALDirectAddressing(PAL_SP));
	}
}

void parserClass::parseCoutTail()
{
	if(t.type == BITLEFT_T)
	{
		t = scanner.getToken();
		parseFollowCout();
		parseCoutTail();
	}
}
void parserClass::parseFollowCout()
{
	switch(t.type)
	{
	case STRING_T:
		{
			string s = t.lexeme;
			for(int i = 1;i<s.length()-1;++i)
			{
				printInstruction(PAL_OUTB,NULL,toPALChar(s[i]));
			}

			t = scanner.getToken();
		}
		break;
	case ENDL_T:
		{
			//Print the next line character
			printInstruction(PAL_OUTB,NULL,toPALLiteral(PAL_NEXT_LINE));
			t = scanner.getToken();
		}
		break;
	default:
		SymbolType resultType = parseExpr();
		checkVariable(resultType);
		//First print the result of the expression
		printInstruction(PAL_OUTW,NULL,toPALDirectAddressing(PAL_R1));
		//Then remove the temp from the top of the stack
		printInstruction(PAL_MOVW,NULL,PAL_R1,NULL,PAL_SP);
	}
}
void parserClass::parseFollowID(SymbolNode* id)
{
	switch(t.type)
	{
	case ASSIGNMENT_T:
		{
			checkVariable(id);
			t = scanner.getToken();
			SymbolType resultType = parseExpr();
			if(id->type != resultType)
				errorAndExit("Cannot assign variable of "+string(symbolTypeStrings[resultType])+(" to "+id->lexeme));
			//First move the temp variable generated by expression parsing to the variable on the left of =
			printInstruction(PAL_MOVW,NULL,toPALDirectAddressing(PAL_R1),id,"");
			//Then remove the temp from the top of the stack
			printInstruction(PAL_MOVW,NULL,PAL_R1,NULL,PAL_SP);
		}
		break;
	case TILDE_T:
		{
			checkVariable(id);
			t = scanner.getToken();
			SymbolNode* rightParam = checkId(t.lexeme);
			checkTokenAndGetNext(t,IDENTIFIER_TOKEN);
			if(rightParam->type != id->type)
				errorAndExit("Cannot swap "+id->lexeme+" and "+rightParam->lexeme+" because they are of different types.");
			//First move the first parameter to the top of the stack
			printInstruction(PAL_MOVW,id,"",NULL,toPALDirectAddressing(PAL_SP));
			//Increase stack pointer
			printInstruction(PAL_ADDW,NULL,toPALLiteral(PAL_WORD_SIZE),NULL,PAL_SP);
			//Then move the second parameter to the first parameter
			printInstruction(PAL_MOVW,rightParam,"",id,"");
			//Then move the first parameter back
			printInstruction(PAL_MOVW,NULL,"-4"+toPALDirectAddressing(PAL_SP),rightParam,"");
			//Then decrease SP to remove the temp
			printInstruction(PAL_SUBW,NULL,toPALLiteral(PAL_WORD_SIZE),NULL,PAL_SP);
		}
		break;
	case LEFTPAREN_T://Function call
		{
			if(id->kind != FUNC_ID)
				errorAndExit(id->lexeme+" is not a function.");
			int expectedParamCount = id->numOfParams;
			t = scanner.getToken();
			parseExpr();
			//TODO: implement function calls in later part of the project, now the expression result is just removed
			printInstruction(PAL_MOVW,NULL,PAL_R1,NULL,PAL_SP);
			int foundParamCount = 1+parseFollowExpr();
			if(expectedParamCount != foundParamCount)
				errorAndExit("Incorrect number of parameters for function ["+id->lexeme+"]. Expecting "+to_string(expectedParamCount)+", found "+to_string(foundParamCount)
				+".");
			checkTokenAndGetNext(t,tokenClass(RIGHTPAREN_T,NONE_ST,")"));
		}
		break;
	default:
		//Check the case when id is a function but does not have any parameter
		if(id->kind==FUNC_ID)
		{
			if(id->numOfParams != 0)
				errorAndExit("Incorrect number of parameters for function ["+id->lexeme+"]. Expecting "+to_string(id->numOfParams)+", found none.");
			printInstruction(PAL_CALL,NULL,toPALLiteral(0),NULL,id->PALLabel);
		}
	}
}

int parserClass::parseFollowExpr()
{
	if(t.type == COMMA_T)
	{
		t = scanner.getToken();
		parseExpr();
		//TODO: use the parameter. Now the result of the expression is just cleared from the stack
		printInstruction(PAL_MOVW,NULL,PAL_R1,NULL,PAL_SP);
		return 1+parseFollowExpr();
	}
	return 0;
	//Do nothing otherwise
}
void parserClass::parseFollowIf()
{
	if(t.type == ELSE_T)
	{
		t = scanner.getToken();
		parseStatement();
	}
	//Do nothing if the token is not ELSE_T
}
SymbolType parserClass::parseExpr()
{
	pStackClass stack;//Stack used in the algorithm.
	stack.push(SEMICOLON_TOKEN);//First push a semicolon onto the stack.
	if(isEndOfExpression(t))
		errorAndExit("Empty expression");
	//mark the current top of the stack with R1
	printInstruction(PAL_MOVW,NULL,PAL_SP,NULL,PAL_R1);
	while(true)
	{
		tokenClass topTerm = stack.getTopMostTerminal();
		//Termination condition.
		//It should be met if the expression is valid and terminate with semicolon
		if(isEndOfExpression(t) && topTerm.type == SEMICOLON_T)
		{
			//Move the entry on top of the stack(the result of the expression) to R1
			printInstruction(PAL_MOVW,stack.top->token.symbol,"",NULL,toPALDirectAddressing(PAL_R1));
			//Move SP back to R1(clear all temp variables created during parsing)
			printInstruction(PAL_MOVW,NULL,PAL_R1,NULL,PAL_SP);
			//Move SP up
			printInstruction(PAL_ADDW,NULL,toPALLiteral(PAL_WORD_SIZE),NULL,PAL_SP);
			//Store the type to be returned before deleting the temp variables in the table
			SymbolType type = stack.top->token.symbol->type;
			scanner.symbolTable.top->deleteTempSymbol();
			return type;
		}
		Precedence p = prec(topTerm,t);//Store the precedence because it’s checked multiple times.
		if(p == LESS_PRECEDENCE || p == EQUAL_PRECEDENCE)//Shift
		{
			stack.push(tokenSymbolClass(t.type,t.subtype,t.lexeme));
			t = scanner.getToken();
		}
		else if(p == GREATER_PRECEDENCE)//Reduce
		{
			std::vector<tokenSymbolClass> tokens;
			stack.lastTerminalPopped = tokenClass(EMPTY_T,EMPTY_ST,EMPTY_LEXEME);//Reset the lastTerminalPopped
			do
			{
				tokenSymbolClass popped = stack.pop();
				if(popped.type == EMPTY_T)//If the stack is empty. This should not happen in normal operation.
					errorAndExit("Empty stack.");
				tokens.push_back(popped);
			}
			while(stack.lastTerminalPopped.type == EMPTY_T
				||(!stack.terminalOnTop())
				||prec(stack.getTopMostTerminal(),stack.lastTerminalPopped) != LESS_PRECEDENCE);

			SymbolNode* symbol = isValidRHS(tokens);
			if(symbol != NULL)
			{
				if(scanner.expressionDebugging)//Print debug message if expression debugging is turned on.
				{
					cout<<"E -> ";
					for(int i = 0;i<tokens.size();++i)
						cout<<tokens[tokens.size()-1-i].lexeme<<" ";
					cout<<endl;
				}
				//Replace the expression in the array with a E if the expression is valid.
				stack.push(tokenSymbolClass(E_T,NONE_ST,"E",symbol));
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
	precedenceTable[tokenToTableIndex(tokenClass(col,NONE_ST,EMPTY_LEXEME))][tokenToTableIndex(tokenClass(row,NONE_ST,EMPTY_LEXEME))] = pre;
}

int parserClass::tokenToTableIndex(tokenClass token)
{
	//Right parenthesis is an end of expression,
	//but we use its original precedence in the table.

	switch(token.type)
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
		if(isEndOfExpression(token))
			return 9;
		//Everything else is considered other.
		return 10;
	}
}

Precedence parserClass::prec(tokenClass firstToken,tokenClass secondToken)
{
	return precedenceTable[tokenToTableIndex(firstToken)][tokenToTableIndex(secondToken)];
}
SymbolNode* parserClass::addTempVariable(SymbolType type)
{

	SymbolNode* node = new SymbolNode(getNextTempName(),VAR_ID,type);
	if(scanner.symbolTable.tableAddEntry(node))
	{
		//Move SP up to give space for the new temp variable
		printInstruction(PAL_ADDW,NULL,toPALLiteral(PAL_WORD_SIZE),NULL,PAL_SP);
		return node;
	}
	return NULL;
}

SymbolNode* parserClass::palArithmetic(SymbolNode* node1, SymbolNode* node2,string op)
{
		SymbolNode* temp = addTempVariable(INT_TYPE);
		//First move the first parameter to temp location
		printInstruction(PAL_MOVW,node1,"",temp,"");
		//Then carry out the arithmetic operation on the temp
		printInstruction(op,node2,"",temp,"");
		return temp;
}


SymbolNode* parserClass::palMod(SymbolNode* node1, SymbolNode* node2)
{
		SymbolNode* temp1 = palArithmetic(node1,node2,PAL_DIVW);
		SymbolNode* temp2 = palArithmetic(node2,temp1,PAL_MULW);
		SymbolNode* temp3 = palArithmetic(node1,temp2,PAL_SUBW);
		return temp3;
}

SymbolNode* parserClass::palAnd(SymbolNode* node1, SymbolNode* node2)
{
	SymbolNode* result = palArithmetic(node1,node2,PAL_MULW);
	result->type = BOOLEAN_TYPE;
	return result;
}

SymbolNode* parserClass::palOr(SymbolNode* node1, SymbolNode* node2)
{
	SymbolNode* andResult = palAnd(node1,node2);
	SymbolNode* addResult = palArithmetic(node1,node2,PAL_ADDW);
	SymbolNode* result = palArithmetic(addResult,andResult,PAL_SUBW);
	result->type = BOOLEAN_TYPE;
	return result;
}

SymbolNode* parserClass::palBranch(SymbolNode* node1, SymbolNode* node2, string op)
{
	//First compare the two parameters
	printInstruction(PAL_CMPW,node1,"",node2,"");
	//Add a temp variable as the result
	SymbolNode* temp = addTempVariable(BOOLEAN_TYPE);
	//First set the temp to be true
	printInstruction(PAL_MOVW,NULL,toPALLiteral(1),temp,"");
	string branchName = getNextTempName();
	//Based on the comparision, either skip to the label, or 
	//carry out the next instruction, which set the temp to false.
	printInstruction(op,NULL,branchName);
	printInstruction(PAL_MOVW,NULL,toPALLiteral(0),temp,"");
	printBranch(branchName);
	return temp;
}

SymbolNode* parserClass::isValidRHS(std::vector<tokenSymbolClass> tokens)
{
	//Base case of a single terminal
	if(tokens.size() == 1)
	{
		tokenClass token = tokens.back();
		if(token.type == INTEGER_T
			||token.type == TRUE_T
			||token.type == FALSE_T)//If token is a literal
		{
			//Create a temp variable in the table and stack based on type of token
			SymbolNode* temp = NULL;
			if(token.type == INTEGER_T)
			{
				temp = addTempVariable(INT_TYPE);
				printInstruction(PAL_MOVW,NULL,toPALLiteral(token.lexeme),temp,"");
			}
			else if(token.type == TRUE_T)
			{
				temp = addTempVariable(BOOLEAN_TYPE);
				printInstruction(PAL_MOVW,NULL,toPALLiteral(1),temp,"");
			}
			else
			{
				temp = addTempVariable(BOOLEAN_TYPE);
				printInstruction(PAL_MOVW,NULL,toPALLiteral(0),temp,"");
			}
			return temp;
		}
		else if(token.type == IDENTIFIER_T)//If token is a variable
		{
			//Here we checks whether the identifier is declared,
			//if the token is an indentifier.
			SymbolNode* temp = checkId(token.lexeme);
			return temp;
		}
		return NULL;
	}
	//All the other cases have three tokens
	if(tokens.size() != 3)
		return NULL;

	//Reversed because of the stack.
	tokenSymbolClass last = tokens[0];
	tokenSymbolClass middle = tokens[1];
	tokenSymbolClass first = tokens[2];
	//First check the case of (E)
	if(first.type == LEFTPAREN_T && last.type == RIGHTPAREN_T)
		return middle.symbol;//Assume middle is valid E.
	//Then check the case of E X E, where X is a YASL operator
	//If the expression at both ends of the expression are valid.
	SymbolNode* node1 = first.symbol;//Assuem first is valid E.
	SymbolNode* node2 = last.symbol;//Assume last is valid E.
	if(node1 == NULL || node2 == NULL)
		return NULL;
	//Make sure both operands are of the same type first.
	if(node1->type != node2->type)
		errorAndExit(node1->lexeme+" and "+node2->lexeme+" are not of the same type.");
	//Make sure both operands are of type int or boolean.
	checkVariable(node1);

	//Check +
	if(middle.type == ADDOP_T && middle.subtype == ADD_ST)
	{
		if(node1->type != INT_TYPE)
			errorAndExit("Addition can only be used with integers.");
		return palArithmetic(node1,node2,PAL_ADDW);
	}
	//Check *
	if(middle.type == MULOP_T && middle.subtype == MULTIPLY_ST)
	{
		if(node1->type != INT_TYPE)
			errorAndExit("Multiplication can only be used with integers.");
		return palArithmetic(node1,node2,PAL_MULW);
	}
	//Check -
	if(middle.type == ADDOP_T && middle.subtype == SUBSTRACT_ST)
	{
		if(node1->type != INT_TYPE)
			errorAndExit("Subtraction can only be used with integers.");
		return palArithmetic(node1,node2,PAL_SUBW);
	}
	//Check div
	if(middle.type == MULOP_T && middle.subtype == DIV_ST)
	{
		if(node1->type != INT_TYPE)
			errorAndExit("Division can only be used used integers.");
		return palArithmetic(node1,node2,PAL_DIVW);
	}
	//Check mod
	if(middle.type == MULOP_T && middle.subtype == MOD_ST)
	{
		if(node1->type != INT_TYPE)
			errorAndExit("Mod can only be used with integers.");
		return palMod(node1,node2);
	}
	//Check or
	if(middle.type == ADDOP_T && middle.subtype == OR_ST)
	{
		if(node1->type != BOOLEAN_TYPE)
			errorAndExit("OR can only be used with booleans.");
		return palOr(node1,node2);
	}
	//Check and
	if(middle.type == MULOP_T && middle.subtype == AND_ST)
	{
		if(node1->type != BOOLEAN_TYPE)
			errorAndExit("AND can only be used with booleans.");
		return palAnd(node1,node2);
	}
	//Check ==
	if(middle.type == RELOP_T && middle.subtype == EQUAL_ST)
	{
		return palBranch(node1,node2,PAL_BEQ);
	}
	//Check <
	if(middle.type == RELOP_T && middle.subtype == LESS_ST)
	{
		return palBranch(node1,node2,PAL_BLSS);
	}
	//Check <=
	if(middle.type == RELOP_T && middle.subtype == LESSOREQUAL_ST)
	{
		return palBranch(node1,node2,PAL_BLEQ);
	}
	//Check >
	if(middle.type == RELOP_T && middle.subtype == GREATER_ST)
	{
		return palBranch(node1,node2,PAL_BGTR);
	}
	//Check >=
	if(middle.type == RELOP_T && middle.subtype == GREATEROREQUAL_ST)
	{
		return palBranch(node1,node2,PAL_BGEQ);
	}
	//Check <>
	if(middle.type == RELOP_T && middle.subtype == UNEQUAL_ST)
	{
		return palBranch(node1,node2,PAL_BNEQ);
	}
	return NULL;
}


void parserClass::recurDescentErrorAndExit(string found, vector<string> expected)
{
	string s = "Found ["+found+"] when expecting ";
	if(expected.size() > 1)
		s += "one of ";
	s += "[";
	for(int i = 0;i<expected.size()-1;++i)
	{
		s += expected[i]+",";
	}
	s += expected[expected.size()-1]+"]";
	errorAndExit(s);
}

void parserClass::errorAndExit(string message)
{
	cout<<"Syntax Error at line ";
	scanner.printCurrentLine();
	cout<<message<<endl;
	scanner.close();
	outfile.close();
	cin.get();
	exit(0);
}

void parserClass::checkTokenAndGetNext(tokenClass token, tokenClass expected)
{
	if(token.type != expected.type)
		recurDescentErrorAndExit(token.lexeme,vector<string>(1,expected.lexeme));
	t = scanner.getToken();
}

SymbolNode* parserClass::checkId(string lexeme)
{
	SymbolNode* result = scanner.symbolTable.tableLookup(lexeme);
	if(result == NULL)
		errorAndExit("Identifier ["+lexeme+"] is undeclared.");
	return result;
}

bool parserClass::isEndOfExpression(tokenClass token)
{
	if(token.subtype == NONE_ST)
	{
		if(token.type == DO_T || 
			token.type == SEMICOLON_T||
			token.type == THEN_T ||
			token.type == BITLEFT_T ||
			token.type == END_T ||
			token.type == ELSE_T ||
			token.type == COMMA_T ||
			token.type == RIGHTPAREN_T)//Note: checking right paren here.
			return true;
	}
	return false;
}

string parserClass::toPALLiteral(int n)
{
	return toPALLiteral(to_string(n));
}

string parserClass::toPALLiteral(string n)
{
	return "#"+n;
}

string parserClass::toPALChar(char c)
{
	if(c == ' ')
		return toPALLiteral(PAL_SPACE);
	//if(c>='A' && c <= 'Z' || c >='a' && c<= 'z')
	string result = "^";
	result += c;
	return result;
}

void parserClass::printInstruction(string instruction, SymbolNode* firstParamPtr, string firstParam, SymbolNode* secondParamPtr, string secondParam)
{
	outfile<<instruction<<" "<<getParameter(firstParamPtr,firstParam)<<" "<<getParameter(secondParamPtr,secondParam)<<endl;
}

void parserClass::printInstruction(string instruction, SymbolNode* firstParamPtr, string firstParam)
{
	outfile<<instruction<<" "<<getParameter(firstParamPtr,firstParam)<<endl;
}

string parserClass::getParameter(SymbolNode* paramPtr, string param)
{
	if(paramPtr == NULL)
		return param;
	string address = "";
	//TODO: now address is only correct for variables defined as global or local)
	//, not for those defined at level 1,2, etc.
	if(paramPtr->nestingLevel == 0)
		address = toPALDirectAddressing(PAL_R0);
	else address = toPALDirectAddressing(PAL_FP);
	string s = "+"+to_string((paramPtr->offset)*PAL_WORD_SIZE)+address;
	return s;
}

string parserClass::toPALDirectAddressing(string memoryLocation)
{
	return "@"+memoryLocation;
}


string parserClass::getNextTempName()
{
	static int nextCt = 0;
	char tempName[10];
	sprintf(tempName,"$%d",nextCt++);
	return tempName;
}

void parserClass::checkVariable(SymbolType type)
{
	if(type != INT_TYPE && type != BOOLEAN_TYPE)
		errorAndExit("Expecting a variable, but found "+string(symbolTypeStrings[type]));
}
void parserClass::checkVariable(SymbolNode* node)
{
	checkVariable(node->type);
}

void parserClass::printBranch(string branchName)
{
	outfile<<branchName<<" ";
}