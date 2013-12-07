//Tao Qian
//This file contains implementations of the 
//fileManagerClass

//The class header is found in filemngr.h

#include "stdafx.h"  // Required for visual studio to work correctly

#include "filemngr.h"


fileManagerClass::fileManagerClass():autoPrintStatus(false)
//Precondition: None.
//Postcondition: The constructor has opened the file which is to be
//               processed and has inialized the currentLineNumber to 0
{  char filename[MAXCELLSPERSTRING];

   //cout << "Enter file name to compile: ";
   //cin.getline (filename, MAXCELLSPERSTRING);
   //fileToCompile.open(filename);
   fileToCompile.open("test.txt");
   
   if (fileToCompile.fail())
   {  cout << "Error, the file: " << filename << " was not opened." << endl;
      cout << "Press return to terminate program." << endl;
      cin.get();
      exit(1);
   }  
  currentLineNumber = 0;
}

int fileManagerClass::getNextChar()
//Precondition:  The source file associated with the owning object has
//               been prepared for reading.
//Postcondition: The next character from the input file has been read and
//               returned by the function.  If another chacter could not
//               be read because END OF FILE was reached then EOF is
//               returned.
{  if ((currentLineNumber == 0) ||
      (charToReadNext == strlen(currentLine)))
   {  if (fileToCompile.peek() == EOF) 
         return (EOF);
	  fileToCompile.getline(currentLine, MAXCELLSPERSTRING);
      strcat(currentLine, "\n");
      currentLineNumber++;
	  //If a new line is read, print it out if print status is true
	  if(autoPrintStatus)
		  printCurrentLine();
      charToReadNext = 0;
   }
   return(currentLine[charToReadNext++]);
}


void fileManagerClass::closeSourceProgram()
//Precondition:  The file belonging to the object owning this routine has
//               been opened.
//Postcondition: The file belonging to the object owning this routine has
//               been closed.
{  fileToCompile.close();
}

void fileManagerClass::pushBack()
//Returning the last character to the buffer.
{
	if(charToReadNext > 0)
		charToReadNext--;
}

void fileManagerClass::printCurrentLine()
//Print the current line(the content of the buffer).
{
	cout<<currentLineNumber<<": "<<endl<<currentLine;
}

void fileManagerClass::setPrintStatus(bool newStatus)
//Set the print status. If set to true, 
//every time a new line is read from the file it is printed.
{
	autoPrintStatus = newStatus;
}

int fileManagerClass::numLinesProcessed()
//returns the cumulative number of lines 
//that have been processed (read into the buffer) so far.
{
	return currentLineNumber;
}
