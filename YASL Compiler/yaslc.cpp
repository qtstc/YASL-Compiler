//Tao Qian
//This driver program should be used to test the first part of the YASL
//compiler.

//This version tested in Visual Studio .NET 2008


#include "stdafx.h"  // A visual studio requirement


#include "parser.h"
#include <iostream>

int main(int argc, char* argv[])
{   


	parserClass parser;
	while (1) //an infinite loop
	{ cout << "About to parse an expression: \n";
		parser.parseExpr();
	  cout << "Parsed one expression\n";
	}

	cin.get();

    return (0);
}

