# ifndef _HUFFMANJN_H
# define _HUFFMANJN_H
# include <stdint.h>
# include <stdbool.h>
# ifndef NIL
# define NIL (void *) 0
# endif

typedef struct DAH treeNode;

struct DAH
{
	uint8_t symbol;
	uint64_t count;
	bool leaf;
	treeNode *left, *right;
};

int getNextBit(uint8_t, uint8_t);

treeNode *newNode(uint8_t s, bool l, uint64_t c);

void delTree(treeNode *t);

static inline void delNode(treeNode *h) {free(h); return; }

static inline int8_t compare(treeNode *l, treeNode *r)
{
	return l->count - r->count;
}

treeNode *join(treeNode *l, treeNode *r);

# endif
