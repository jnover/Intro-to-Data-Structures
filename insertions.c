# include <stdint.h>
# include "huffman.h"
# include "queue.h"

/**
 * insertionSort function.
 *
 * sorting the queue with 
 * insertion sort!
 *
 * @param array[]       the array to sort.
 * @param len           the length of the array.
 * 
 */

void  insertionSort (queue *q, uint32_t head, uint32_t tail)
{

	treeNode ** array = q->Q; 

        uint32_t i;                                     	// i is the variable for the outer
                                                        	// loop.
        int j;                                          	// j is the variable for the inner
                                                        	//loop.
        treeNode *temp;                                  	//temp is the variable to store the
                                                       		//item that we're currently looking at.
	
	for(i = head+1; i < tail; i++)                        	//for loop to loop through the values.
        {
		temp = array[i];                        	//set the current one equal to temp.
                j = i - 1;                              	//j starts from one before current 
                	                                       	//value.
                while( j > -1 &&  array[j]->count > temp->count)//while j is equal or greater than 0 in counts
               	{
                	array[j+1] = array[j];          	//push things back if they are greater
                      	j--;                            	//than temp.
               	}

                array[j+1] = temp;                      	//make the final move! put temp in
        }
       
	 return;
}

