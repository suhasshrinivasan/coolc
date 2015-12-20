/* This file uses data structures and function declared under lexer.h to perform the actual lexical analysis. */ 

#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"
// #include "myhash.h"

#define BUFLEN 256   // Allowed length per line

/* Global function declarations. */

/* Auxiliary procedures for getToken().
 * See below, definitions of the procedures for their descriptions.
 * Declarations, in order of definitions.
 */
char getNextChar ();
void ungetChar ();
void figureStartState (char , bool* , StateType* , TokenType* );
TokenType getSpecialSymbol (char);
void figureInID (char , bool* , StateType* , TokenType* );
void figureInNum (char , bool* , StateType* , TokenType* );
void figureInHyphen (char , bool* , StateType* , TokenType* );
void figureInlessthan (char , bool* , StateType* , TokenType* );
void figureInParen (char , bool* , StateType* , TokenType* );
void figureInComment (char , bool* , StateType* , TokenType* );
void figureEndComment (char , bool* , StateType* , TokenType* );
void figureInString (char , bool* , StateType* , TokenType* ); 
void figureInSlash (char* , bool* , StateType* , TokenType* , int* ); 
TokenType keywordLookup (char* );
TokenType linearLookup (char* );

/* getToken, principle procedure. */
TokenType getToken(); 

/* END global function declarations. */
/* --------------------------------- */


/* Global variable declarations. */

char lineBuffer[BUFLEN];	// char buffer to hold the current line.
unsigned int lineNumber;	// number of the current line.
unsigned int linePos;	// position in the current line.
unsigned int bufferSize;    // current size of the current lineBuffer.

/* To distinguish between '--[text]--' comments and '(*[text]*)' comments.
 * It is set true for former and false for latter. 
 */
bool commentFlag;  

char TokenString[][50] = {"ID", "NUM", "STRING", "COMMENT", "PLUS", "MINUS", "MUL", "DIV", "OPEN_PAREN", "CLOSE_PAREN", "OPEN_CURLY_BRACE", 
"CLOSE_CURLY_BRACE", "OPEN_SQUARE_BRACE", "CLOSE_SQUARE_BRACE", "COLON", "SEMICOLON", "COMMA", "ASSIGN", "EQUIVALENCE", "TILDE", "GREATER_THAN", 
"LESS_THAN_EQUAL", "LESS_THAN", "NOT", "DOT", "AT_RATE_OF", "_CLASS", "_ELSE", "_FALSE", "_FI", "_IF", "_IN", "_INHERITS", "_ISVOID", "_LET", 
"_LOOP", "_POOL", "_THEN", "_WHILE", "_CASE", "_ESAC", "_NEW", "_OF", "_NOT", "_TRUE", "ERROR"};

KeywordRecord KeywordList[NUMBEROFKEYWORDS] = {{"class", _CLASS}, {"else", _ELSE}, {"false", _FALSE}, {"fi", _FI}, {"if", _IF}, {"in", _IN}, {"inherits", _INHERITS}, {"isvoid", _ISVOID},
   {"let", _LET}, {"loop", _LOOP}, {"pool", _POOL}, {"then", _THEN}, {"while", _WHILE}, {"case", _CASE}, {"esac", _ESAC}, {"new", _NEW},
   {"of", _OF}, {"not", _NOT}, {"true", _TRUE}};

/* For single lookahead in parsing. */
TokenType prevToken = _CLASS;
int prevLinePos = 0;
TokenType* currentTokenPointer = NULL;

/* END global variable declarations. */
/* --------------------------------- */


/* Auxiliary procedure definitions. */

/* getNextChar fetches the next non-blank character from the input stream. */ 
char getNextChar()
{
	/* If linePos is less than the bufferSize (length of current line)
	 * then just return the current characted present in 
	 * lineBuffer and then increment linePos to point to the 
	 * next value in the lineBuffer. 
	 */
	if (linePos < bufferSize) {
		return lineBuffer[linePos++];
	}

	/* If linePos is greater than or equal to length of the bufferSize
	 * then increment lineNumber, as we traverse the next line
	 */
	lineNumber++;

	/* If the current characted present in lineBuffer is not EOF yet then 
	 * update bufferSize by the length of the current line, update linePos 
	 * to 0, return the current characted present in lineBuffer and then 
	 * increment linePos to point to the next value in the lineBuffer.
	 */

	 if (fgets (lineBuffer, BUFLEN - 1, source))
	 {
	 	bufferSize = strlen (lineBuffer);
	 	linePos = 0;
	 	return lineBuffer[linePos++];
	 }
	 /* If the current characted present in lineBuffer is EOF
	  * then return EOF.
	  */
	 lineNumber--;
	 return EOF;
} // end getNextChar 

/* Backracks one character in the input stream. */
void ungetChar() // decrements linpos
{
	linePos--;
} // end ungetChar


/* keyWordLookup will call hashLookup or linearLookup based on ENHASH which is defined in lexer.h. */
TokenType keywordLookup (char* key)
{
	// if (ENHASH)
	// {
	// 	HashChainNode* result = hashLookup (hashHead, key);
	// 	if (result)
	// 		return result -> hashedKeyword.token;
	// 	return ID;
	// }

	return linearLookup (key);
}

/* Linear search.
 * FURTHER OPTIMIZATION POSSIBLE USING HASH TABLES!  
 */ 
TokenType linearLookup (char* key)
{
	printf ("Lookup debug\n");
	int i;
	for (i = 0; i < NUMBEROFKEYWORDS; i++)
	{
		// printf ("key = %s\tKeywordList[%d].keywordValue = %s\n", key, i, KeywordList[i].keywordValue);
		if (!strcmp(key, KeywordList[i].keywordValue))
		{
			// printf ("KeywordList[%d].token = %d\n", i, KeywordList[i].token);
			return KeywordList[i].token;
		}
	}
		

	return ID;
} // end LinearKeywordLookup


/* figureStartState lays out the configuration of the START state. */
void figureStartState(char c, bool* save, StateType* state, TokenType* currentToken)
{
	/* first two cases start ID defintion */
	if (isalpha(c) || c == '_')
	{
		*save  = true;
        *state = INID;
    }
    /* on encounter of a digit, NUM definition starts */
    else if (isdigit(c))
    {
		*save  = true;
        *state = INNUM; 
    }
    else
    {
		switch(c)
		{
			/* '"' defines INSTRING. */
			case '\"':
				*save  = false;
				*state = INSTRING;
				break;

			/* '(' needs a lookahead.
			 * If '*' is encountered, it is a multiline comment.
			 * Otherwise, just an OPEN_PAREN
			 */
			case '(':
				*save   	= false;
				*state  	= INPAREN;
				commentFlag = false;
				break;

	        /* these three cases define white spaces and we don't need to store them */
			case ' ' :
			case '\n':
			case '\t': 
			case '\r':
			//~ case EOF:
	            *save = false; 
	            break;

		    /* '-' needs single lookahead. 
			 * If another '-' is encountered then it starts to define an INCOMMENT state, otherwise it defines MINUS. */
			case '-':
	            *save  = true; 
	            *state = INHYPHEN; 
	            break;

			/* '<' needs single lookahead as well. 
			 * If another '-' is encountered then it defines an INASSIGN STATE, or '=' defines LESS_THAN_EQUALS, otherwise LESS_THAN */
		    case '<': 
	            *save  = true;
	            *state = INLESSTHAN; 
	            break;
		  
		    /* here we define all the special symbols that do not require any lookahead */
		    default: 
	            *save         = true;
			    *state        = DONE;
				*currentToken = getSpecialSymbol(c);
	            break;
	    }
    }
} // end figureStartState


/* getSpecialSymbol() determines if the given character is one of those symbols which do not require a lookahead and returns it.
 * If if isn't any of the special symbols defined in the language then it returns a ERROR TokenType. 
 */
TokenType getSpecialSymbol(char c)  
{
    switch(c)
    { 
	    case '=': /* printf ("currentToken = %c %d:%d\n", c, lineNumber, linePos); */
	    		  return EQUIVALENCE;
	    case ',': /* printf ("currentToken = %c %d:%d\n", c, lineNumber, linePos); */ 
	              return COMMA;
		case ':': /* printf ("currentToken = %c %d:%d\n", c, lineNumber, linePos); */
		          return COLON;
		case '+': /* printf ("currentToken = %c %d:%d\n", c, lineNumber, linePos); */
		          return PLUS;
		case '*': /* printf ("currentToken = %c %d:%d\n", c, lineNumber, linePos); */
		          return MUL;
		case '/': /* printf ("currentToken = %c %d:%d\n", c, lineNumber, linePos); */
		          return DIV;
	    case '(': /* printf ("currentToken = %c %d:%d\n", c, lineNumber, linePos); */
	    	      return OPEN_PAREN;
		case ')': /* printf ("currentToken = %c %d:%d\n", c, lineNumber, linePos); */
                  return CLOSE_PAREN;
		case '{': /* printf ("currentToken = %c %d:%d\n", c, lineNumber, linePos); */
			      return OPEN_CURLY_BRACE;
		case '}': /* printf ("currentToken = %c %d:%d\n", c, lineNumber, linePos); */
				  return CLOSE_CURLY_BRACE;
		case '[': /* printf ("currentToken = %c %d:%d\n", c, lineNumber, linePos); */
				  return OPEN_SQUARE_BRACE;
		case ']': /* printf ("currentToken = %c %d:%d\n", c, lineNumber, linePos); */
                  return CLOSE_SQUARE_BRACE;
		case ';': /* printf ("currentToken = %c %d:%d\n", c, lineNumber, linePos); */
		          return SEMICOLON;
		case '~': /* printf ("currentToken = %c %d:%d\n", c, lineNumber, linePos); */
		          return TILDE;
        case '.': /* printf ("currentToken = %c %d:%d\n", c, lineNumber, linePos); */
                  return DOT; 
		case '@': /* printf ("currentToken = %c %d:%d\n", c, lineNumber, linePos); */
		          return AT_RATE_OF; 
		case '>': /* printf ("currentToken = %c %d:%d\n", c, lineNumber, linePos); */
		          return GREATER_THAN;
		default : /* printf ("currentToken = %c %d:%d\n", c, lineNumber, linePos); */
		          return ERROR; 
    }
} // end getSpecialSymbol

/* figureInID. INID state configurations. */
void figureInID(char c, bool* save, StateType* state, TokenType* currentToken)
{
	/* If c is other than '_' and alphanumeric, i.e., any other symbol, then return ID. */
	if(c != '_' && !isalnum(c))
	{
		ungetChar();
		*save 		  = false;
		*state 		  = DONE;
		*currentToken = ID;
	}
} // end figureInID.

/* figureInNum. INNUM state configurations. */
void figureInNum(char c, bool* save, StateType* state, TokenType* currentToken)
{
	/* If c is other than a digit, return number. */
	if(!isdigit(c))
	{
		ungetChar();
		*save 		  = false;
		*state 		  = DONE;
		*currentToken = NUM;
	}
} // end figureInNum

/* figureInHyphen. 
 * When a single '-' is encountered in the source program, INHYPHEN state is reached.
 * In INHYPHEN state, there is a requirement of a lookahead.
 * '--' defines an INCOMMENT.
 * '-[other]' defines a MINUS TokenType.
 * Based on the lookahead, save, state and currentToken is altered. 
 */
void figureInHyphen(char c, bool* save, StateType* state, TokenType* currentToken)
{
    switch(c)
    {
        /* '--' defines INCOMMENT */
        case '-': 
            *save       = false;
            *state      = INCOMMENT;
            commentFlag = true;
            break;

        /* '-[other]' defines MINUS */
        default:
            *save         = false; 
            *state        = DONE;
            *currentToken = MINUS;
            ungetChar();
            break;
    }
} // end figureInHyphen


/* figureInlessthan. 
 * When a single '<' is encountered in the source program, INLESSTHAN state is reached.
 * In INLESSTHAN state, there is a requirement of a lookahead.
 * '<-' defines a ASSIGN TokenType.
 * '<=' defines a LESS_THAN_EQUALS TokenType.
 * '<[other]' defines LESS_THAN TokenType.
 * Based on the lookahead, save, state, currentToken is altered. 
 */
void figureInlessthan(char c, bool* save, StateType* state, TokenType* currentToken)
{
    switch(c)
    {
        /* '<-' defines ASSIGN TokenType */
        case '-': 
            *save         = true;
            *state        = DONE; 
            *currentToken = ASSIGN;
            break;
                        
        /* '<=' defines less than or equal to operator */
        case '=':
            *save         = true;
            *state        = DONE;
            *currentToken = LESS_THAN_EQUAL;
            break;

        /* '<[other]' defines less than token */
        default:
            *save         = false;
            *state        = DONE;
            *currentToken = LESS_THAN;
            ungetChar();
            break;
    }
} // end figureInlessthan


/* figureInParen.
 *'(' needs a lookahead.
 * If '*' is encountered, it is a multiline comment.
 * Otherwise, just an OPEN_PAREN
 */
void figureInParen(char c, bool* save, StateType* state, TokenType* currentToken)
{
	switch(c)
	{
		/* Beginning of multiline comment. */
		case '*': 
			*save  		= false;
			*state 		= INCOMMENT;
			commentFlag = false;
			break;

		/* If ([other] then, its a OPEN_PAREN. */
		default:
			*save 		  = false;
			*state 		  = DONE;
			*currentToken = OPEN_PAREN;
			ungetChar();
			break;
	}
}

/* figureInComment.
 * If commentFlag = false and in INCOMMENT state, another '*' is encountered, comment is ended.
 * If commentFlag = true and in INCOMMENT state, another '-' is encountered, a single lookahead is required to judge comment end.
 */
void figureInComment(char c, bool* save, StateType* state, TokenType* currentToken)
{
	/* This case defines '(*[text]*' which is end of comment. */
	if(commentFlag == false && (c == EOF || c == '*'))
	{
		*save         = false;
		*state        = ENDCOMMENT;
	}

	/* This case defines --[text]EOF or --[text]\n. */
	else if(commentFlag == true && (c == EOF || c == '\n'))
	{
		*state 		  = START;
	}
} // end figureInComment

/* figureEndComment.
 * When in ENDCOMMENT state, if another '-' is encountered, it defines end of comment. 
 * Otherwise the comment text is continued to be scanned. 
 */
void figureEndComment(char c, bool* save, StateType* state, TokenType* currentToken)
{
	switch(c)
	{
		case ')':
			*state = START;
			break;

		default: 
			*state = INCOMMENT;
			break;
	}
} // end figureEndComment

/* figureInString.
 * Strings are enclosed in "[text]".
 * Within a string, a seq \c denotes char 'c',
 * with the exception of escape sequence chars.
 */
void figureInString(char c, bool* save, StateType* state, TokenType* currentToken)
{
	if(c == '\\')
	{
		*save  = true;
		*state = INSLASH;
	}
	else if(c == '\"')
	{
		*save 		  = false;
		*state 	      = DONE;
		*currentToken = STRING;
	}
	else if(c == EOF || c == '\n')
	{
		*save         = false;
		*state 		  = DONE;
		*currentToken = ERROR;
	}
	else
		*save = true;
} 

/* figureInSlash.
 * If escape seq char is encountered, then remove prev \ and append seq. 
 * If it is another \ then stay.
 * Otherwise, revert back to INSTRING.
 */
void figureInSlash(char* c, bool* save, StateType* state, TokenType* currentToken, int* tokenIndex)
{
	switch(*c)
	{
		case 'n':
			*c = '\n';
			*save = true;
			(*tokenIndex)--;
			*state = INSTRING;
			break;

		case 'b':
			*c = '\b';
			*save = true;
			(*tokenIndex)--;
			*state = INSTRING;
			break;
		
		case 'f':
			*c = '\f';
			*save = true;
			(*tokenIndex)--;
			*state = INSTRING;
			break;

		case 't':
			*c = '\t';
			*save = true;
			(*tokenIndex)--;
			*state = INSTRING;
			break;

		case '\\':
			break;

		default:
			*state = INSTRING;
	}

}

/* END auxiliary function definitions. */
/* ----------------------------------- */


/* getToken definition */

/* TokenType getToken() is the principle function of the lexer. 
 * It identifies and returns the TokenType of the current token.
 */ 
TokenType getToken()
{
	int tokenIndex = 0;       // walker for scanning the source
	TokenType currentToken;   // holds current token type to be returned
	StateType state = START;  // current state - always begin with START
	bool save;                // flag to indicate save to tokenString
	currentTokenPointer = &currentToken;
	
	prevLinePos = linePos;

	while(state != DONE)
	{
		char c = getNextChar();  // increments linepos and gets the next char from source
		
		if(c == EOF) {
			return _EOF;
		}
		
		switch(state)
		{
		    case START:
		    	figureStartState(c, &save, &state, &currentToken);
			    break;

			case INID:
				figureInID(c, &save, &state, &currentToken);
				break;

			case INNUM:
				figureInNum(c, &save, &state, &currentToken);
				break;
			  
			case INHYPHEN:
                figureInHyphen(c, &save, &state, &currentToken);
                break;
				    
			case INLESSTHAN:
                figureInlessthan(c, &save, &state, &currentToken);
				break;
            
			case INPAREN:
				figureInParen(c, &save, &state, &currentToken);
				break;

			case INCOMMENT:
				figureInComment(c, &save, &state, &currentToken);
				break;

			/* Comment end for '(*[text]*)' type. */
		    case ENDCOMMENT:
		    	figureEndComment(c, &save, &state, &currentToken);
		    	break;

		    case INSTRING:
		    	figureInString(c, &save, &state, &currentToken);
		    	break;

		    case INSLASH:
		    	figureInSlash(&c, &save, &state, &currentToken, &tokenIndex);
		    	break;
		}

		if (save && tokenIndex <= MAXTOKENLENGTH) 
		{
			tokenString[tokenIndex++] = c;
			// printf ("tokenString[%d] = %c\tlinePos = %d\n", tokenIndex - 1, tokenString[tokenIndex - 1], linePos);
		}

		if (state == DONE)
		{
			tokenString[tokenIndex] = '\0';
			if(currentToken ==  ID)
			{
				currentToken = keywordLookup(tokenString);
				if(currentToken == ERROR)
				{
					printf ("currentToken = %s %d:%d tokenID = %d\n", TokenString[currentToken], lineNumber, linePos + 1, currentToken);
					return currentToken;
				}
				else
				{
					printf ("currentToken = %s %d:%d tokenID = %d\n", TokenString[currentToken], lineNumber, linePos + 1, currentToken);
					return currentToken;
				}
			}
		}
    }
    
    printf ("currentToken = %s %d:%d tokenID = %d\n", TokenString[currentToken], lineNumber, linePos + 1, currentToken);
    // printf ("Token = %s\nTokenString[currentToken] = %s\nTokenString[prevToken] = %s\nLine number = %d\nLine pos = %d\nPrev pos = %d\n\n", tokenString, TokenString[currentToken], TokenString[prevToken], lineNumber, linePos, prevLinePos);
    return currentToken;
} // end getToken

/* ungetToken. */
TokenType ungetToken()
{
	linePos = prevLinePos;
	(*currentTokenPointer) = prevToken;
	printf ("currentToken = %s %d:%d tokenID = %d\n", TokenString[prevToken], lineNumber, linePos, prevToken);
	//printf ("Token = %s\nTokenString[currentToken] = %s\nTokenString[prevToken] = %s\nLine number = %d\nLine pos = %d\nPrev pos = %d\n\n", tokenString, TokenString[*currentTokenPointer], TokenString[prevToken], lineNumber, linePos, prevLinePos);
	return prevToken;
}
