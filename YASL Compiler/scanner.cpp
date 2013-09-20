//Tao Qian
//This file contains implementations of the 
//ScannerClass and TokenClass

//The class header is found in scanner.h

#include "stdafx.h"  // Required for visual studio to work correctly
#include "scanner.h"

TokenClass::TokenClass():type(EMPTY_T),subtype(EMPTY_ST),lexeme(EMPTY_LEXEME)
{}

TokenClass::TokenClass(int type, int subtype, string lexeme)
{
	this->type = type;
	this->subtype = subtype;
	this->lexeme = lexeme;
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
	for(int i = 0;i<MAX_STATE;i++)
		for(int j = 0;j<MAX_CHAR;j++)
			stateMatrix[i][j] = INVALID_STATE;

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
