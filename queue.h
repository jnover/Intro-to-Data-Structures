/**
 * Yu Jung
 *
 * queue.h
 *
 * structure given to us by DDEL from the assignment page.
 */

# ifndef _QUEUE_H
# define _QUEUE_H

# include "stackLibs.h"
# include <stdint.h>
# include <stdbool.h>
# include "huffman.h"

typedef struct queue
{
	uint32_t size;				//how big is it?
	uint32_t head, tail;			//front and rear locations
	treeNode **Q;				//array to hold it(via calloc)
}queue;

/**
 * newQ function.
 *
 * creates a new queue and
 * returns a pointer to it.
 *
 * @param size the size of the
 * queue.
 *
 * @return q the newly created
 * queue. 
 */
static inline queue *newQ(uint32_t size)
{
	queue *q = (queue *)(malloc(sizeof(queue)));		//malloc because will be throwing it
								//around a lot to different functions.
	q->size = size;						//set the size to the size given
	q->head = 0;						//set both tail & head at the start
	q->tail = 0;
	q->Q = (treeNode **)(malloc(size * sizeof(treeNode *)));//lastly, create a smaller bucket inside
								//the big bucket.
	return q;		
}

/**
 * emptyQ function.
 *
 * tells if the specified
 * queue is empty or not.
 *
 * @param q the queue
 *
 * @return true if it's 
 * empty , false if its not.
 */
static inline bool emptyQ(queue *q)
{
	return (q->head == q->tail);				//it's really when they're equal to each other
}

/**
 * fullQ function.
 *
 * tells if the queue is full
 * or not.
 *
 * @param q the queue to be checked
 *
 * @return true if the queue is full
 * or false if it's not.
 */
static inline bool fullQ(queue *q)
{
	return (q->head == ((q->size + q->tail +1) % q->size));	//if the queue wraps around and has only one free space
								//I'm quite sad that I didn't get to implement the circular
								//queue, I'll do it as soon as I can figure out a good
								//way to do it.
}

bool enQ(queue *, treeNode *);

void delQ(queue *);

/**
 * deQ function.
 *
 * dequeues an item
 * at the head of the queue.
 *
 * @param q the queue to pop from
 * @param t the place to store the
 * popped one
 *
 * @return the dequeued item.
 */
static inline bool deQ(queue *q, treeNode **t)
{
	if(emptyQ(q))						//if the function is empty
	{
		return false;					//tell the user you have no boys in there
	}
	treeNode *popped = q->Q[q->head];			//otherwise, pop one from the head
	(q->Q)[q->head] = NULL;					//set that position to NULL so that we don't have
								//two pointers to the same thing lying around
	q->head = (q->size + q->head +1) % q->size;		//increment the head
	(*t) = popped;						//store the popped one in the tree node place given
	return true;						//return we got one
}

/**
 * printQ function.
 *
 * used exclusivelly for debugging.
 * prints the queue.
 *
 * @param q the queue to be printed
 */
static inline void printQ(queue *q)
{
	printf("Queue size : %u\n", q->size);
	printf("Queue head : %u\n", q->head);
	printf("Queue tail : %u\n", q->tail); 
	
	uint32_t i;
	for(i = 0; i < q->size; i++)
	{
		printf("[");
		if((q->Q)[i])
			printNode((q->Q)[i]);	
		printf("]\t\n");
	} 
	printf("\n");	
}

/**
 * oneLeft function.
 *
 * answers the question
 * if there's only one left
 * in the queue.
 *
 * @param q the queue to be
 * checked
 *
 * @return true if yes
 * false if no
 *
 */
static inline bool oneLeft(queue *q)
{								
	return (((q->head + q->size + 1)%q->size) == q->tail);	//it's really when head + 1 = tail.
}

# endif
