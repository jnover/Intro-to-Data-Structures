/**
 * Yu Jung
 *
 * queue.c
 */

# include "queue.h"

/**
 * enQ function.
 *
 * puts something in the 
 * speficied queue.
 *
 * @param q the specified
 * queue
 * @param add the object to add
 * 
 * @return true if successfully added
 * false if not
 */
bool enQ(queue *q, treeNode *add)
{
        if(fullQ(q))						//if the queue is full
        {							//tell the programmer we fit
                return false;					//no more
        }
        else							//otherwise,
        {
                (q->Q)[q->tail] = add;				//add it to the tail
                q->tail = (q->size + q->tail + 1) % q->size;	//increment the tail to be pointing
								//at a blank space
                return true;					//return yes we fit them
        }
}

/**
 * delQ function.
 *
 * deletes a queue.
 *
 * @param q the queue to be deleted.
 */
void delQ(queue *q)
{
	treeNode *t;						//the treeNode temporary variable
	while(deQ(q, &t))
	{
		delTree(t);					//delete each tree.
	}
	
	free(q->Q);						//free the bucket after the water
	free(q);						//free the larger bucket holding the smaller bucket
	
}
