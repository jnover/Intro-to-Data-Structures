/**
 * Yu Jung.
 * 04.22.2017
 *
 * bv.c.
 */

# include "bv.h"
# include <stdlib.h>
# include <stdint.h>
# include <math.h>
# include <stdio.h>
// Code bv.h from DDEL 4/20/17

/**
 * newVec function.
 *
 * returns the pointer to a newly born bit vector!
 *
 * @param len the designated length.
 */
bitV* newVec(uint32_t len)					
{
	bitV *vector = malloc(sizeof(bitV));			//memory allocates for a bitvector since
	vector->l = len;					//this is returning a pointer. 
	vector->v = calloc(len/8+1, sizeof(uint8_t));		//set l to the designated length while
								//allocating more memory for v, the actual bits
	return vector;						//representing the primes.
}


/**
 * delVec function.
 *
 * frees the space allocated inside the vector
 * as well as the vector itself after the program.
 *
 * @param vector the pointer to the vector i'm 
 * trying to free.
 */
void delVec(bitV *vector)
{
	free(vector->v);					//free the v part first,
	free(vector);						//and then free the actual vector.
	return;	
}

/**
 * one function.
 *
 * sets every bit to 1 in the bit vector.
 *
 * @param vector the pointer to the bit vector.
 */
void one(bitV *vector)
{
	uint32_t len = vector->l;				//store the length in a variable since
	len = len/8+1;						//i don't want to write vector->l all the time.
								//divide by 8 and add 1 so that it properly reaches
								//the whole bitvector.

		
	uint32_t i;						//for the loop below.						
	for(i = 0; i < len; i++)				//set every number to 255, which is
	{							//the biggest number any one uint8_t can represent.
		vector->v[i] = 255;
	}
	return;
	
}

/**
 * setBit function.
 *
 * sets one tiny specific bit in the whole bit vector
 *
 * @param vector the pointer to the vector
 * @param index the index of the tiny bit 
 */
void setBit( bitV *vector, uint32_t index)
{
	uint32_t whichbox = index/8;				//which box variable is for literally which box,
								//the index of v[].
	uint32_t remainder = index%8;				//remainder is used to calculate and navigate
								//within that box.
	remainder = 7-remainder;				//since i'm visualizing it so that 2 to the 7
								//represents if 0 is prime or not, i have to
								//subtract it from 7 for it to flip around the 
								//box.
	uint8_t binary = vector->v[whichbox];			//because i don't like long statements. i end up
								//having to position my comments way to the side
								//if my statements are too long.
	binary |= (0x1<<remainder);				//use the binary math thing to successfully set
								//that one bit.
	vector->v[whichbox] = binary;				//make it equal to the value in the bitvector.
	
	return;
}

/**
 * clrBit function.
 *
 * friend of the setBit function.
 * clears the tiny bit instead.
 *
 * @param vector the pointer to the vector
 * @param index the index of the tiny bit
 */
void clrBit(bitV *vector, uint32_t index)
{
	uint32_t whichbox = index/8;				//again, which box meaning which box.
	uint32_t remainder = index%8;				//remainder same as above.

	remainder = 7 - remainder;				//this is all really same except for the 
								//binary math part
	uint8_t binary = vector->v[whichbox];			//i store the value again, in variable "binary".
	
	
	binary &= ~(0x1<<remainder);				//binary math. i flip around the bits after
	vector->v[whichbox] = binary;				//i shift and do the &.
	return;
	
}

uint8_t valInt(bitV *vector, uint32_t index)
{
	return vector->v[index];		
}

/**
 * valBit function.
 *
 * returns the value of one tiny bit.
 * used for seeing if some number index is
 * prime or not.
 *
 * @param vector the pointer to the vector
 * @param index the index of some number
 */
uint8_t valBit(bitV * vector, uint32_t index)
{
	uint32_t whichbox = index/8;				//which box?
	uint32_t remainder = index%8;				//remainder!
	remainder = 7-remainder;				//i just thought it would be easier for me to
								//write functions if they were all in a straight
	uint8_t binary = vector->v[whichbox];			//line... and it was a lot easier this way for
								//me to visualize it as well
	binary &= (0x1<<remainder);				//set every other value other than the one i'm 
								//looking for to 0
	return (binary >> (remainder));				//now, does that value equal the value it's supposed
}								//to equal? i.e. is it one in that place only and
								//nowhere else?

/**
 * lenVec function.
 *
 * simply returns the length.
 * so simple.
 *
 * @param vector the pointer to the vector
 */
uint32_t lenVec (bitV *vector)
{
	return vector->l;					//this one's easy; just return the length!
}
