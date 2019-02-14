# ifndef _STACKJN
# define _STACKJN
# include <stdint.h>
# include "huffmanJN.h"
# include <stdbool.h>

// Code from DDEL;
// Modified to work for treeNode structures.

typedef treeNode *item;

typedef struct stack
{
		uint64_t size;
		uint64_t top;
		item *entries;
} stack;

stack *newStack(uint32_t);

void delStack(stack *);

item pop(stack *);

void push(stack *, item);

bool empty(stack *);

# define INVALID 0xDeadD00d
# endif
