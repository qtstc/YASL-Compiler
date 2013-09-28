//Dave A. Berque, Revised August 2011
//This file contains some general constant definitions that will
//be useful in several parts of the compiler.  Feel free to add
//additional definitions here.

#ifndef _general
#define _general

const int MAXCELLSPERSTRING = 82;  //80 characters as well as \n plus \0.

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

//Here we define the type of keywords as the sume of KEYWORD_BASE and an integer.
//The sequence of the keywords in this list should not be changed
//because the number are mapped to an array.
#define KEYWORD_BASE 160
#define PROGRAM_T KEYWORD_BASE
#define FUNCTION_T KEYWORD_BASE + 10
#define BEGIN_T KEYWORD_BASE + 20
#define END_T KEYWORD_BASE + 30
#define IF_T KEYWORD_BASE + 40
#define THEN_T KEYWORD_BASE + 50
#define ELSE_T KEYWORD_BASE + 60
#define WHILE_T KEYWORD_BASE + 70
#define DO_T KEYWORD_BASE + 80
#define COUT_T KEYWORD_BASE + 90
#define CIN_T KEYWORD_BASE + 100
#define ENDL_T KEYWORD_BASE + 110
#define INT_T KEYWORD_BASE + 120
#define BOOLEAN_T KEYWORD_BASE + 130
#define TRUE_T KEYWORD_BASE + 140
#define FALSE_T KEYWORD_BASE + 150

#define EOF_T 320

#define MAX_STATE 16
#define MAX_CHAR 129//EOF is -1. We store it as 128.
#define EOF_INDEX 128//Index of EOF in the matrix

#define INVALID_STATE -1

enum Action {NO_ACTION,ACCEPT,ERROR,WARNING,CLEAR_BUFFER,CHECK_COMPILER_DIRECTIVE};

#endif