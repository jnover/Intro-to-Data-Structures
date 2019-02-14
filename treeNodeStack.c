/**
 * Yu Jung.
 *
 * treeNodeStack.c
 */

# include "treeNodeStack.h"

/**
 * delTNS function.
 *
 * deletes the specified TNS.
 *
 * @param tree the specified TNS
 * to delete.
 */
void delTS(tnStack *tree)
{
        while(!emptyTS(tree))				//while the tree is not empty
        {
		treeNode *temp = NIL;
		popTS(tree, &temp);			//store the tree node popped
							//off the tree.
                delNode(temp);				//delete that node
        }
        free(tree->entries);				//free the smaller bucket
        free(tree);					//now the larger bucket.
}

/**
 * pushTS function.
 *
 * pushes something on
 * to the stack (a treeNode.)
 *
 * @param tree the tree to
 * push it on
 * @param node the node to be
 * pushed on
 */
bool pushTS(tnStack *tree, treeNode *node)
{
        if(fullTS(tree))				//if the tree is full
        {
                return false;				//no can do
        }

        tree->entries[tree->top] = node;		//otherwise, push it on
							//the top
        tree->top += 1;					//increment the top so it always
							//points to a blank space
        return true;					//return all good
}

/**
 * popTS function.
 *
 * pops a treenode off
 * the stack.
 *
 * @param tree the tree
 * to pop it off from
 * @param t the place to store
 * the treenode popped.
 *
 */
bool popTS(tnStack *tree, treeNode **t)
{
        if(emptyTS(tree))				//if it's empty
        {
                return false;				//no can do
        }
        tree->top--;					//otherwise, decrement top first
							//so it'll actually point at something
        treeNode *ret = tree->entries[tree->top];	//pop that one off
        tree->entries[tree->top] = NULL;		//make sure to set the popped space to 
							//null so that we won't have two pointers on the
							//stack accidentally. even though if I leave it
							//it'll be overwriten (probably) I like to 
							//make sure.
        (*t) = ret;					//store the returning popped one onto place t.

        return true;					//return all good
}
