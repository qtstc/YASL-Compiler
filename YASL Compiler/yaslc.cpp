//Tao Qian
//This driver program should be used to test the first part of the YASL
//compiler.

//This version tested in Visual Studio .NET 2008


#include "stdafx.h"  // A visual studio requirement


#include "scanner.h"
#include <iostream>

int main(int argc, char* argv[])
{   
	ScannerClass scanner;
	TokenClass token = scanner.getToken();
	while(token.type != EOF_T)
	{
		cout<<token.lexeme<<"\t\t"<<TokenClass::tokenIntToString(token.type)<<"\t\t"<<TokenClass::tokenIntToString(token.subtype)<<endl;
		token = scanner.getToken();
	}
	cout<<token.lexeme<<endl;
	//scanner.printStateMatrix();
	scanner.close();
	cin.get();

    return (0);
}

