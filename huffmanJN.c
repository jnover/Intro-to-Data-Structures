# include <stdint.h>
# include <stdio.h>
# include <stdbool.h>
# include <stdlib.h>
# include "huffmanJN.h"

int getNextBit(uint8_t x, uint8_t pos)
{
	return (x >> (pos % 8)) & 1;
}

treeNode *newNode(uint8_t s, bool l, uint64_t c)
{
	treeNode *new = malloc(sizeof(treeNode));
	
	new->symbol = s;
	new->count = c;
	new->leaf = l;
	new->left = NIL;
	new->right = NIL;

	return new;
}

void delTree(treeNode *root)
{
	if(root->left != NULL) 
	{
		delTree(root->left);
	}
	if(root->right != NULL)
	{
		delTree(root->right);
	}
	free(root);
	return;
}

int16_t stepTree(treeNode *root, treeNode **t, uint8_t code)
{
	if(code == 0) *t = (*t)->left;
	if(code == 1) *t = (*t)->right;
	if((*t)->leaf) 
	{
		*t = root;
		return (*t)->symbol;
	}
	else return -1;
}

treeNode *join(treeNode *l, treeNode *r)
{
	treeNode *tree = newNode('$', 0, 0);
	tree->left = l;
	tree->right = r;
	
	return tree;
}
