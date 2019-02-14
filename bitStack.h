/**
 * Yu Jung
 *
 * bitStack.h
 */

# ifndef _STACK_BITS
# define _STACK_BITS

# ifndef NIL
# define NIL (void *)0
# endif

# include "stackLibs.h"
# include "bv.h"

										//i'm just thinking how its really funny
										//because i've decided to use bits that i
typedef struct bitstack								//literally get to have function names like
{										//fullBS and newBS... like here's a new BS...
	uint32_t size;								//and here's one full of BS... anyway
	uint32_t top;
	bitV *entries;								//will hold everything using bitvectors!
}bitStack;

/**
 * newBS function.
 * 
 * creates a new bitStack.
 *
 * @param len the length of
 * the bitVector.
 */
static inline bitStack * newBS(uint32_t len)
{
	bitStack *bits = (bitStack *)(malloc(sizeof(bitStack)));		//allocating space for the actual object
	bits->size = len;
	bits->top = 0;
	bits->entries = newVec(len);						//create a new vector!
	return bits; 								//bits remind me of starbits. like in 
										//super mario galaxy.
										//thus i'm really fond of bits and i
										//went an extra steps because i really
										//wanted to use (star)bits.
}

/**
 * delBS function.
 *
 * deletes a bitStack.
 *
 * @param bits the bitStack
 * to be deleted.
 */
static inline void delBS( bitStack *bits)
{
	delVec(bits->entries);							//free the water!
	free(bits);								//free the bucket.
	return;
}

void pushBS(bitStack *, uint8_t);						//i just thought pushing is too long
										//for a static inline. maybe its because
										//i realloc more memory each time its full.

uint8_t popBS(bitStack *);

/**
 * emptyBS function.
 *
 * sees if the bitStack is
 * empty or not.
 *
 * @param bits the bitStack
 * to check on.
 *
 * @return bool depending on the
 * result.
 */
static inline bool emptyBS(bitStack *bits)					
{
	return (bits->top==0); 							//woop... no bs here...
}

/**
 * fullBS function.
 *
 * sees if the bitStack is
 * full or not.
 *
 * it better not be full of BS, though
 *
 * @param bits the bitStack to
 * check on.
 *
 * @return bool depending on the 
 * result.
 */
static inline bool fullBS(bitStack *bits)
{
	return (bits->top > bits->size);					//if this was full of bs my program
}										//wouldn't be working


/**
 * printBS function.
 *
 * prints out the bitStack.
 *
 * @param bits the bitStack 
 * to be printed.
 */
static inline void printBS(bitStack *bits)
{
	if(emptyBS(bits))							//again, seg faulted here
	{									//i just didn't wanna get confused using
		printf("----------------\n");					//uint32_t's and ints so i just defaulted to
		return;								//uint32_t's... and look what happens
	}
	
	uint32_t i;
	for(i = 0; i < bits->top; i++)					//loop to print everything
	{
		printf("%u", valBit(bits->entries, i));
	}
	//printf("%u\n", valBit(bits->entries, 0));
										//the bottom line!
} 

/**
 * peekBS function.
 *
 * lets you peek an index
 * on the bitStack.
 *
 * this kind of defeats the purpose of
 * putting it into a stack, but I
 * gotta do it to copy code.
 *
 * @param bits the bitStack we're
 * trying to peek into
 * @param index the index that we 
 * want to peek
 */
static inline uint8_t peekBS(bitStack *bits, uint32_t index)
{
	return valBit(bits->entries, index);
}


/**
 * copyCode function.
 *
 * Initializes all of the
 * bitStack array storing the
 * code for encoding and also
 * stores the code in there.
 *
 * passing a double pointer because
 * all of the array slots are null
 * right now and if we want to change the
 * value specifically inside the array
 * we need a double pointer.
 *
 * @param code the place to be
 * storing code.
 * @param follow the place to
 * copy the code from.
 */
static inline void copyCode(bitStack **code, bitStack *follow)
{
	uint32_t top = follow->top;
	
        (*code) = newBS(top);

        while(follow->top !=  0)
        {
                pushBS((*code), popBS(follow));
        }

        follow->top = top;

}

# endif

