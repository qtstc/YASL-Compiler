//Dave A. Berque, Revised August 2011
//This file contains some general constant definitions that will
//be useful in several parts of the compiler.  Feel free to add
//additional definitions here.

#ifndef _general
#define _general

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

#define BITWISE_T 40
#define BITLEFT_ST 41
#define BITRIGHT_ST 42

#define LEFTPAREN_T 50
#define RIGHTPAREN_T 60

#define SEMICOLON_T 70
#define COMMA_T 80
#define DOT_T 90
#define TILDE_T 100
#define STRING_T 110
#define IDENTIFIER_T 120
#define INTEGER_T 130
#define ASSIGNMENT_T 140
#define AMPERSAND_T 150 

#define PROGRAM_T 160
#define FUNCTION_T 170
#define BEGIN_T 180
#define END_T 190
#define IF_T 200
#define THEN_T 210
#define ELSE_T 220
#define WHILE_T 230
#define DO_T 240
#define COUT_T 250
#define CIN_T 260
#define ENDL_T 270
#define INT_T 280
#define BOOLEAN_T 290
#define TRUE_T 300
#define FALSE_T 310

#define OTHER_T 320//Used in the precedence table.
#define E_T 330//Used in shift reduce parser.

#define EOF_T 340

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

#endif