#ifndef _COMMON_H
#define _COMMON_H

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

typedef struct KeywordStruct
{
	char* keywordValue;
	TokenType token;
} KeywordRecord;

#define M 				 31     // Hashtable size. Number of keywords.

#endif

