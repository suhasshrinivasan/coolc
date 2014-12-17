#ifndef _HASH_H
#define _HASH_H

#include "lexer.h"

/* hashchain */
typedef struct hashchainnode
{
	KeywordRecord hashedKeyword;
	struct hashchainnode* nextHashEntry;
} HashChainNode;

/* hashhead */
typedef struct hashmaphead
{
	HashChainNode** hashArray;
	unsigned int bucketCount;
} HashMapHead;

HashMapHead* hashHead;

/* Modular hash function. */
unsigned int hash(char* );

/* Function to create our hashtable. */ 
HashMapHead* createHashTable(unsigned int );

/* Inserting new key into the hashtable. */
void hashInsert(HashMapHead* , KeywordRecord );

/* Lookup function. */
HashChainNode* hashLookup(HashMapHead* , char* );

/* Deleting a table. Freeing up the memory. */
void hashFree(HashMapHead* );

#endif
