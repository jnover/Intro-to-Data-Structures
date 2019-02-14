/**
 * Yu Jung
 *
 * huffman.h
 * 
 * structure given by DDEL on the assignment page.
 */

# ifndef _HUFFMAN_H
# define _HUFFMAN_H

# include <stdint.h>
# include <stdbool.h>
# include <ctype.h>
# include <stdlib.h>
# include <unistd.h>

# include "bitStack.h"

# ifndef NIL
# define NIL (void *) 0
# endif

typedef struct DAH treeNode;

struct DAH
{
	uint8_t symbol;
	long long unsigned int count;
	bool leaf;
	uint32_t depth;
	treeNode *left, *right;
};

// New node, with symbols,leaf or not , a count associated with it
static inline treeNode * newNode ( char s, uint64_t c, bool l)
{
	treeNode *tn = (treeNode *)(malloc(sizeof(treeNode)));
	tn->symbol = s;
	tn->count = c;
	tn->leaf = l;
	tn->left = tn->right = NULL;
	tn->depth = 1;
	return tn;		
}

// Delete a single node
static inline void delNode (treeNode *t)
{
	free(t);
}
//Delete a tree
static inline void delTree ( treeNode *t)
{
	treeNode *ptr = t;
	if(ptr->left != NULL)
	{
		ptr = ptr->left;
		delTree(ptr);
	}
	if(ptr->right != NULL)
	{
		ptr = ptr->right;
		delTree(ptr);
	}
	if(ptr->right == NULL && ptr->left == NULL)
	{
		free(ptr);
		return;	
	}
		
}

// Parse a Huffman tree to build codes
// is a combination of building codes and dumping tree onto a file.
void buildCode (treeNode *, bitStack*[], int);

static inline bool compare ( treeNode *l, treeNode *r)
{
	return l->count - r-> count; // l < r if negative , l = r if 0 , ...
}

static inline void join ( treeNode *l, treeNode *r, treeNode **j)// Join two subtrees
{
	treeNode *in = newNode('$', l->count + r->count, false);
	in->left = l;	
	in->right = r;
	(*j) = in;
	
	if(!l->leaf)
	{
		r->depth = l->depth;
		(*j)->depth = l->depth + 1; 	
	}
	else if(!r->leaf)
	{
		l->depth = r->depth;
		(*j)->depth = l->depth + 1;
	}
	else
	{
		(*j)->depth = 2;	
	}

}

/**
 * areYouMyDad function.
 *
 * returns the parent node of 
 * the node specified by the 
 * bit pattern in the follow.
 *
 * name derived from one of the
 * Steven Universe episodes,
 * "Are You My Dad".
 *
 * I am my mom!
 *
 * @param follow the bitStack for
 * storing the path to  here
 * @param head the head of the tree.
 */
static inline treeNode *areYouMyDad(bitStack *follow, treeNode *head)
{
	uint8_t addback = popBS(follow);			//called add back because
								//it'll be added back later to 
								//cause no trouble.
	treeNode *dad = head;					//dad starts at the head.
	uint32_t i;
	for(i = 0; i < follow->top; i++)			//for loop to go through the
	{							//tree, to find the parent of  
		if(peekBS(follow,i) == 1)			//the tree node.
		{
			dad = dad->right;			//right if next bit is 1,
		}
		else
		{
			dad = dad->left;			//left if 0.
		}	
	}

	pushBS(follow, addback);				//then add the bit back in
	return dad;						//and return dad.
}

/**
 * printNode function.
 *
 * used exclusively for debugging.
 *
 * @param t the node to print.
 */
static inline void printNode(treeNode *t)
{
	if(isalnum(t->symbol))					//check if the character can be
	{							//printed as a character.
		printf("\tSymbol : %c\t", t->symbol);
	}
	else							//otherwise, print it as a hex.
	{
		printf("\tSymbol : %X\t", (int)t->symbol);
	}
	printf("Count : %llu\t", t->count);
}

# endif

