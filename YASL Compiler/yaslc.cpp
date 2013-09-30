//Tao Qian
//This driver program should be used to test the first part of the YASL
//compiler.

//This version tested in Visual Studio .NET 2008


#include "stdafx.h"  // A visual studio requirement


#include "parser.h"
#include <iostream>

int main(int argc, char* argv[])
{   
	//scannerClass scanner;
	//tokenClass token = scanner.getToken();
	//while(token.type != EOF_T)
	//{
	//	cout<<token.lexeme<<"\t\t"<<tokenClass::tokenIntToString(token.type)<<"\t\t"<<tokenClass::tokenIntToString(token.subtype)<<endl;
	//	token = scanner.getToken();
	//}
	//cout<<token.lexeme<<endl;
	////scanner.printStateMatrix();
	//scanner.close();

	parserClass parser;
	while (1) //an infinite loop
	{ cout << "About to parse an expression: \n";
		parser.parseExpr();
	  cout << "Parsed one expression\n";
	}


	/*pStackClass stack;
	tokenClass c(11,12,"haha");
	stack.push(c);
	tokenClass t = stack.pop();
	cout<<t.type<<endl;*/

	cin.get();

    return (0);
}

