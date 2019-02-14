# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <stdbool.h>
# include "stack.h"

// Code from DDEL
// Modified to work with treeNode structures;

stack *newStack(uint32_t size)
{
	stack *s = (stack *) calloc(size, sizeof(stack));
	s->size = size;
	s->top  = 0;
	s->entries = (item *) calloc(size, sizeof(item));
	return s;
}

void delStack(stack *s)
{ 
	free(s->entries);
	free(s); 
	return;
}

item pop(stack *s)
{
		if (s->top > 0)
		{
			s->top -= 1;
			return s->entries[s->top];
		}
		else
		{
			return NULL;
		}
}

void push(stack *s, item i)
{
		if (s->top == s->size)
		{
			s->size *= 2;
			s->entries = (item *) realloc(s->entries, s->size * sizeof(item));
		}
		s->entries[s->top] = i;
		s->top += 1;
		return;
}

bool empty(stack *s)
{
		return s->top == 0;
}
