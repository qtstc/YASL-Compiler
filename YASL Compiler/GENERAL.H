//Dave A. Berque, Revised August 2011
//This file contains some general constant definitions that will
//be useful in several parts of the compiler.  Feel free to add
//additional definitions here.

#ifndef _general
#define _general

#include <sstream>

const int MAXCELLSPERSTRING = 82;  //80 characters as well as \n plus \0.

//Definition used in scanner.cpp

//Below are constant used to represent token type/subtypes.
#define EMPTY_T -10
#define EMPTY_ST -11
#define EMPTY_LEXEME ""

#define NONE_ST 1

#define ADDOP_T 10
#define ADD_ST 11
#define SUBSTRACT_ST 12
#define OR_ST 13

#define MULOP_T 20
#define MULTIPLY_ST 21
#define AND_ST 22
#define DIV_ST 23
#define MOD_ST 24

#define RELOP_T 30
#define EQUAL_ST 31
#define UNEQUAL_ST 32
#define GREATEROREQUAL_ST 33
#define LESSOREQUAL_ST 34
#define GREATER_ST 35
#define LESS_ST 36

#define BITLEFT_T 40
#define BITRIGHT_T 50

#define LEFTPAREN_T 60
#define RIGHTPAREN_T 70

#define SEMICOLON_T 80
#define COMMA_T 90
#define DOT_T 100
#define TILDE_T 110
#define STRING_T 120
#define IDENTIFIER_T 130
#define INTEGER_T 140
#define ASSIGNMENT_T 150
#define AMPERSAND_T 160 

#define PROGRAM_T 170
#define FUNCTION_T 180
#define BEGIN_T 190
#define END_T 200
#define IF_T 210
#define THEN_T 220
#define ELSE_T 230
#define WHILE_T 240
#define DO_T 250
#define COUT_T 260
#define CIN_T 270
#define ENDL_T 280
#define INT_T 290
#define BOOLEAN_T 300
#define TRUE_T 310
#define FALSE_T 320

#define OTHER_T 330//Used in the precedence table.
#define E_T 340//Used in shift reduce parser.

#define EOF_T 350

#define PROGRAM_KEYWORD Keyword(PROGRAM_T,"program")
#define FUNCTION_KEYWORD Keyword(FUNCTION_T,"function")
#define BEGIN_KEYWORD Keyword(BEGIN_T,"begin")
#define END_KEYWORD Keyword(END_T,"end")
#define IF_KEYWORD Keyword(IF_T,"if")
#define THEN_KEYWORD Keyword(THEN_T,"then")
#define ELSE_KEYWORD Keyword(ELSE_T,"else")
#define WHILE_KEYWORD Keyword(WHILE_T,"while")
#define DO_KEYWORD Keyword(DO_T,"do")
#define COUT_KEYWORD Keyword(COUT_T,"cout")
#define CIN_KEYWORD Keyword(CIN_T,"cin")
#define ENDL_KEYWORD Keyword(ENDL_T,"endl")
#define INT_KEYWORD Keyword(INT_T,"int")
#define BOOLEAN_KEYWORD Keyword(BOOLEAN_T,"boolean")
#define TRUE_KEYWORD Keyword(TRUE_T,"true")
#define FALSE_KEYWORD Keyword(FALSE_T,"false")

#define MAX_STATE 16
#define MAX_CHAR 129//EOF is -1. We store it as 128.
#define EOF_INDEX 128//Index of EOF in the matrix

#define INVALID_STATE -1
enum Action {NO_ACTION,ACCEPT,ERROR_ACTION,WARNING_ACTION,CLEAR_BUFFER_ACTION,CHECK_COMPILER_DIRECTIVE_ACTION};


//definition used in parser.cpp
enum Precedence {GREATER_PRECEDENCE,LESS_PRECEDENCE,EQUAL_PRECEDENCE,ERROR_PRECEDENCE};
#define PRECEDENCE_TABLE_DIMENSION 11

//definition used in table.cpp
enum SymbolKind{INVALID_KIND,FUNC_ID,VAR_ID,REF_PARAM,VALUE_PARAM};
const char* const symbolKindStrings[] = {
	"invalid-kind",
	"func-id", 
	"var-id",
	"ref-param",
	"val-param"
};
enum SymbolType{INVALID_TYPE,FUNC_ID_TYPE,INT_TYPE,BOOLEAN_TYPE};
const char*  const symbolTypeStrings[] = {
	"invalid-type",
	"func-id-type",
	"int-type",
	"boolean-type"
};

template <typename T>
std::string to_string(T value)
{
	std::ostringstream os ;
	os << value ;
	return os.str() ;
}

#define IDENTIFIER_TOKEN tokenSymbolClass(IDENTIFIER_T,NONE_ST,"identifier")
#define SEMICOLON_TOKEN tokenSymbolClass(SEMICOLON_T,NONE_ST,";")

//Constants for PAL
#define PAL_WORD_SIZE 4
#define PAL_ADDW "addw"
#define PAL_MOVW "movw"
#define PAL_MULW "mulw"
#define PAL_SUBW "subw"
#define PAL_DIVW "divw"
#define PAL_SP "SP"
#define PAL_INW "inw"
#define PAL_OUTW "outw"
#define PAL_OUTB "outb"
#define PAL_BEQ "beq"//equal
#define PAL_BNEQ "bneq"//not equal
#define PAL_BLSS "blss"//less than
#define PAL_BGTR "bgtr"//greater than
#define PAL_BGEQ "bgeq"//greater equal
#define PAL_BLEQ "bleq"//less equal
#define PAL_CMPW "cmpw"
#define PAL_R1 "R1"
#define PAL_R0 "R0"

#endif