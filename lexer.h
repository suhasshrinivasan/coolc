/* This file contains data structures and associated functions required for lexical analysis. 
 * Code is well commented for readability.
 */

#ifndef _LEXER_H
#define _LEXER_H

#include <stdio.h>
//#include "myhash.h"

#define MAXTOKENLENGTH   400 // LOOK INTO THIS
#define MAXKEYWORDLENGTH 8
#define NUMBEROFKEYWORDS 19
#define BUFSIZE			 256
// #define M 				 19     // Hashtable size. Number of keywords.
// #define ENHASH           false  // enable hashLookup for keywordLookup.

/* tokenString contains the current token value. */
char tokenString[MAXTOKENLENGTH+1];

/* source is the pointer to the source file. */
FILE* source;

/* Hashtable. */
//HashMapHead* hashHead;

/* DFA state assignments */
typedef enum
{ 
	START, INASSIGN, INCOMMENT, ENDCOMMENT, INNUM, INID, INLESSTHAN, INHYPHEN, INPAREN, INSTRING, INSLASH, DONE
} StateType;

/* token type assigments */
typedef enum
{ 
	ID, NUM, STRING, COMMENT,
	PLUS, MINUS, MUL, DIV, 
	OPEN_PAREN, CLOSE_PAREN, OPEN_CURLY_BRACE, CLOSE_CURLY_BRACE, 
	OPEN_SQUARE_BRACE, CLOSE_SQUARE_BRACE, COLON, SEMICOLON, 
	COMMA, ASSIGN, EQUIVALENCE, TILDE, 
	GREATER_THAN, LESS_THAN_EQUAL, LESS_THAN, NOT, 
	DOT, AT_RATE_OF,
	// Key words
	_CLASS, _ELSE, _FALSE, _FI, _IF, _IN, _INHERITS, _ISVOID, _LET, _LOOP, _POOL, _THEN, _WHILE, _CASE, _ESAC, _NEW, _OF, _NOT, _TRUE,
	// Unidentified token
	ERROR, _EOF
} TokenType;

/* Token String list */
extern char TokenString[][50];

/* Keywords table */
typedef struct KeywordStruct
{
	char* keywordValue;
	TokenType token;
} KeywordRecord;

/* Keyword List */
KeywordRecord KeywordList[NUMBEROFKEYWORDS];

/* stderr. */
unsigned int lineNumber;		// number of the current line.
unsigned int linePos;		// position in the current line.

/* For single lookahead in parsing. */
TokenType prevToken;
int prevLinePos;
TokenType* currentTokenPointer;

/* token record structure */
typedef struct TokenAttributeStruct
{
	TokenType tokenval;
	union	{
		char* stringValue;
		int numValue;
	} attribute;
} TokenRecord;


/* TokenType getToken() is the principle function of the lexer. 
 * It identifies and returns the TokenType of the current token.
 */ 
TokenType getToken();

/* getNextChar() gets the next character from the currently being scanned source file line. */
char getNextChar();

/* ungetChar() backtracks one  in the currently being scanned source file line. */
void ungetChar();

#endif
