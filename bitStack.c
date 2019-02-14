/**
 * Yu Jung.
 *
 * bitStack.c
 */

# include "bitStack.h"
# include "bv.h"

/**
 * pushBS function.
 *
 * pushing a bit onto the
 * bitStack.
 *
 * @param bits the bitStack to 
 * push the bit on
 * @param the bit
 */
void pushBS(bitStack *bits, uint8_t insbit)
{
	if(fullBS(bits))								//make sure to allocate enough
	{										//before we make mess!
		bits->size += 30;							
		bits->entries = (bitV *)(realloc(bits->entries, bits->size/8+1));
		if(bits->entries == NULL)						//throw an error message if
		{									//that goes nuts
			perror("CANNOT ALLOC MORE SPACE:BS PUSH");
			exit(errno);							//exit gracefully
		}	
	}
	
	if(insbit == 1)									//threre can only be 0 or 1
	{										//if its 1, set the bit
		setBit(bits->entries, bits->top);
	}
	
	else										//if 0, clear the bit
	{
		clrBit(bits->entries, bits->top);
	}
											//the reason why i have to 
											//literally clear a bit is that
											//this also works similar as charStack
											//that it doesn't clear something when
											//popped, and its just overwriting things
	bits->top++;	
	return; 	
}


/**
 * popBS function.
 * 
 * returns the last particular
 * bit.
 *
 * @param bits the bitStack to
 * pop the bits from.
 *
 * @return a bit  
 */
uint8_t popBS(bitStack *bits)
{
        if(emptyBS(bits))                                                       	//we don't want top to
        {                                                                      		//get really really mad and
                return -1;                                                      	//blow up, knowing uint32_t's.
        }

        bits->top--;
        return valBit(bits->entries, bits->top);                                	//i like how i don't have to
                                                                                	//write anything because everything's
                                                                                	//in bv.c already.
} 
