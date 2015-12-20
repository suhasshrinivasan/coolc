/* Driver to test lexer.
 * In order to test, compile using the following shell command:
 * gcc -o lexer.o lexer.c lexer.h lexerDriver.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "common.h"
#include "lexer.h"
#include "myhash.h"
 
KeywordRecord KeywordList[] = {{"class", _CLASS}, {"else", _ELSE}, {"false", _FALSE}, {"fi", _FI}, {"if", _IF}, {"in", _IN}, {"inherits", _INHERITS}, {"isvoid", _ISVOID},
   {"let", _LET}, {"loop", _LOOP}, {"pool", _POOL}, {"then", _THEN}, {"while", _WHILE}, {"case", _CASE}, {"esac", _ESAC}, {"new", _NEW},
   {"of", _OF}, {"not", _NOT}, {"true", _TRUE}}; 

/* Pointer to point to source file. */
FILE* src;

/* Keyword hashtable */
//~ extern HashMapHead* hashHead;

/* isValidFileFormat checks if the input to the compiler is a .cl file. */
bool isValidFileFormat (char* filename)
{
	char* format = filename + strlen (filename) - 3;
	if (!strcmp (format, ".cl")) {
		return true;
	}
	return false;
} // end validInputFileFormat

int main(int argc, char* argv[])
{
	
	   
	if (!isValidFileFormat(argv[1]))	// if the format of the input file is not .cl, exit.
	{
		printf ("%s: file format not recognized\n", argv[1]);
		exit (1);
	}

	 if (ENHASH)
	 {
	 	hashHead = createHashTable (M);	// M is defined in common.h
	 	int i;
	 	for (i = 0; i < M; i++) {
	 		hashInsert (hashHead, KeywordList[i]);	// KeywordList is defined in lexer.h
		}
		printf("Hashtable: Ready\n");
	 }

	printf ("Opening Source file\n");
	src    = fopen (argv[1], "r");
	source = src;	// set source as src to pass the input source file to one that is accessible by lexer.h

	TokenType result = getToken();
	while (!feof(source))
	{
		prevToken = result;
		//printf ("%d\n", result);
		result = getToken();
		//printf ("Next Token\n");
	}
}
