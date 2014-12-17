#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "myhash.h"
//#include "lexer.h"

/* Insertion into the hash chain. */
HashChainNode* listInsertFront (HashChainNode* , KeywordRecord );

/* Linear search hash chain. */
HashChainNode* linearSearch (HashChainNode* , char* );

/* Modular hash function. */
unsigned int hash (char* key)
{
	unsigned int i;
	unsigned int hashValue = 0;

	for (i = 0; key[i] != '\0'; i++)
		hashValue += key[i] << 4;	

	return hashValue % M;	// M available globally in lexer.h
} // end hash.

/* Creates hashtable of size 'size'. */ 
HashMapHead* createHashTable (unsigned int size)
{
	HashMapHead* hashHead = NULL;

	if (size < 1)
	{
		printf("Hash size less than 1!\n");
		return NULL;
	}

	if (hashHead = (HashMapHead*) malloc (sizeof (HashMapHead)))
	{
		if (hashHead -> hashArray = (HashChainNode**) malloc (sizeof (HashChainNode*) * size))
		{
			int i;
			for (i = 0; i < size; i++)
				hashHead -> hashArray[i] = NULL;
			hashHead -> bucketCount = size;
		}
	}

	return hashHead;
} // end createHashTable

/* Hashtable insertion. */
void hashInsert (HashMapHead* hashHead, KeywordRecord keyword1)
{
	if (!hashHead) 
	{
		printf("Hashtable not created!\n");
		return;
	}

	unsigned int bucketNumber = hash (keyword1.keywordValue);
	hashHead -> hashArray[bucketNumber] = listInsertFront (hashHead -> hashArray[bucketNumber], keyword1);
} // end hashInsert.

/* Insertion into the hash chain. */
HashChainNode* listInsertFront (HashChainNode* chainNode, KeywordRecord keyword)
{
	HashChainNode* temp = (HashChainNode*) malloc (sizeof (HashChainNode));
	temp -> hashedKeyword = keyword;
	temp -> nextHashEntry = NULL;

	if (!chainNode)
		return temp;

	temp -> nextHashEntry = chainNode;
	chainNode =  temp;
	return chainNode;
} // hashInsertFront.

/* Lookup function. */
HashChainNode* hashLookup (HashMapHead* hashHead, char* value)
{
	if (!hashHead) 
	{
		printf("Hashtable not created!\n");
		return;

	}

	unsigned int bucketNumber = hash (value);
	return linearSearch (hashHead -> hashArray[bucketNumber], value);
} // end hashLookup.

/* Linear search hash chain. */
HashChainNode* linearSearch (HashChainNode* chainNode, char* value)
{
	if (!chainNode) return NULL;

	HashChainNode* walker = chainNode;
	while (walker)
	{
		if (strcmp (((walker->hashedKeyword).keywordValue), value))
			return walker;

		walker = walker -> nextHashEntry;
	}

	return NULL;
} // end linearSearch.

/* Deleting a table. Freeing up the memory. */
void hashFree(HashMapHead* hashHead)
{
	if (!hashHead) return;

	HashChainNode* walker = NULL;
	HashChainNode* temp   = NULL;

	int i;
	for (i = 0; i < hashHead -> bucketCount; i++)
	{
		walker = hashHead -> hashArray[i];
		while (walker)
		{
			temp = walker;
			walker = walker -> nextHashEntry;
			free (temp -> nextHashEntry);
			free (temp);
		}
	}

	free (hashHead -> hashArray);
	free (hashHead);
} // end hashFree.
