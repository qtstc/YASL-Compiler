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
#define KEYWORD_T 160
#define EOF_T 170

#define MAX_STATE 16
#define MAX_CHAR 129//EOF is -1. We store it as 128.
#define EOF_INDEX 128//Index of EOF in the matrix

#define INVALID_STATE -1

enum Action {NO_ACTION,ACCEPT,ERROR,WARNING,CLEAR_BUFFER,CHECK_COMPILER_DIRECTIVE};

#endif