//Tao Qian
//This driver program should be used to test the first part of the YASL
//compiler.

//This version tested in Visual Studio .NET 2008


#include "stdafx.h"  // A visual studio requirement


#include "scanner.h"
#include <iostream>

int main(int argc, char* argv[])
{   
	/*
	fileManagerClass theFile;     //Define the sourceprogram object
    char ch;

	theFile.setPrintStatus(true);
    while ((ch = theFile.getNextChar()) != EOF){}
	cout<<"YASLC-TQ has just compiled "<<theFile.numLinesProcessed()<< " lines of code.";
    theFile.closeSourceProgram();
	*/


	ScannerClass scanner;
	scanner.printStateMatrix();
	scanner.close();
	//char c = ' ';
	//cout<<(int)c;
	cin.get();

    return (0);
}

