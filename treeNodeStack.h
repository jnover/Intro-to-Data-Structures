/**
 * Yu Jung.
 *
 * treeNodeStack.h
 */

# ifndef _STACK_TREENODE
# define _STACK_TREENODE 

# include "stackLibs.h"
# include "huffman.h"

# ifndef NIL
# define NIL (void *)0
# endif 

typedef struct tnStack								//what is a tnStack?
{
	uint32_t size;								//it has a size
	uint32_t top;								//a top
	treeNode **entries;							//and the actual array of treeNodes!
}tnStack;

/**
 * newTNS function.
 *
 * creates a new treeNode stack.
 *
 * @param len the length of the array.
 */
static inline tnStack *newTS(uint32_t len)
{
	tnStack *tree = (tnStack *)(malloc(sizeof(tnStack)));			//malloc the structure first
	tree->size = len;							//of couse, set size to designated size,
	tree->top = 0;								//top to 0, blah blah
	tree->entries = (treeNode **)(malloc(len * sizeof(treeNode *)));	//a smaller bucket to actually hold the
										//tree nodes
										//I quit using the item thing because 
										//it was kind of bothering me...
	return tree;
}

void delTS(tnStack *);

bool pushTS(tnStack *, treeNode *);

bool popTS(tnStack*, treeNode **);
  
/**
 * fullTS function.
 *
 * answers the question is
 * the tree node stack full or not
 *
 * @param tree the stack to be checked
 *
 * @return yes if true no if false 
 */
static inline bool fullTS(tnStack *tree)
{
	return (tree->top - 1 == tree->size);  					//it really is just if top
										//pointing at a blank space is 
										//larger than size
}

/**
 * emptyTS function.
 *
 * answers the question if
 * the tree node stack empty
 * or not.
 *
 * @param tree the stack to be checked
 *
 * @return YEET if true or no if false
 */
static inline bool emptyTS(tnStack *tree)
{
	return (tree->top == 0);						//again. simple.
}

# endif
