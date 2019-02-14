/**
 * Yu Jung
 *
 * Monday, May 22, 2017
 *
 * encode.c
 */

# include "encode.h"

bool nothing = false;											//bool for accouting
													//when there's nothing in
													//the input file
bool fulltree = false;											//bool for enabling 
													//the full tree

int main(int argc, char **argv)
{

/**==================================reading in options =============================================**/

	int opt;											//all the switch variables
	struct stat fileStat;										//and the variables
	int in = -2;
	int out = -2;
	bool verbose = false;
	bool printt = false;

	while((opt = getopt(argc, argv, "i:o:vp")) != -1)						//getting the option flags
	{
		switch(opt)										
		{
			case 'i':									//for the input file, I didn't
			in = open(optarg, O_RDONLY);   							//really bother copying the string
			if(in == -1)									//somewhere, I just opened the
			{										//file in place
				perror("Cannot open file to read");					//error handling 
				strerror(errno);						
				exit(errno);
			}

			break;
			
			case 'o':									//same for the output file;
			out = open(optarg, O_RDWR | O_CREAT | O_EXCL, 0666);				//I just create it right in the flags
			if(out == -1)									//666 is the best number
			{
				perror("Cannot open/create output");
				strerror(errno);
				exit(errno);
			}
			break;
		
			case 'v':									//turns the verbose option true
			verbose = true;		
			break;


			case 'p':									//turns print tree true
			printt = true;
			break;

			case 'f':									//turns full tree true.
			fulltree = true;								//this had to be a global var
			break;										//because I made this at the end and
		}											//another function needed to use this
	}


/**======================================= error testing & getting file size=========================**/

	
	if(in == -2)											//if the input value did not change								
	{												//from the beginning
		perror("could not find input");								//throw an error and exit
		strerror(errno);									//I was going to have it read in from			
		exit(errno);										//the standard in, but it seemed 
	}												//a bit too hard since I didn't originally
													//plan for it to do that.
													

	if(out == -2)											//if the output did not change 
	{												//set it as the standard out
		out = STDOUT_FILENO;								
	}

	if(fstat(in, &fileStat) < 0)									//get file stat
	{
		perror("ERROR ON FILESTAT");
		strerror(errno);
		exit(errno);
	}



/**======================================= memory mapping the input ==================================**/


	
	uint8_t *input = mmap(NULL, fileStat.st_size, PROT_READ, MAP_PRIVATE, in, 0);  			//memory mapping the file 
	
	if(input == MAP_FAILED)										//if it fails, then it might just 
	{												//be that the file is empty
		if(fileStat.st_size == 0)
		{											 
			nothing = true;
		}
		else											//otherwise, spit out an error
		{											//and exit.
			perror("cannot memory map the file");
			strerror(errno);
			exit(errno);
		}
	}


/**==================================== writing basic info (magic number & file size) ================**/
	
	uint32_t mNo = 0xdeadd00d;									//the magic number
  
	write(out, &mNo, sizeof(mNo));									//writing the magic number
													//to the file

	if(verbose)											//print the original file size
	{
		printf("Original file : %lu bits: ", fileStat.st_size * 8); 				//in bits (multiply by 8)
	}

	
	write(out, &fileStat.st_size, sizeof(fileStat.st_size));					//write to the file the size 


/**======================================== histogram ===============================================**/



	uint64_t histogram[256] = {0};									//create the base for the histogram
 
	int hc = 0;											//headcount for how many leaves there are
	
	histoCount(input, fileStat.st_size, histogram, &hc); 						//pass it to histogram counting function.

	uint16_t treeCount = (3 * hc) - 1;								//calculate the size of the tree								
	write(out, &treeCount, sizeof(treeCount));							//write to the output

/**===================================== queues, then making a tree ====================================**/


	queue *entries = newQ(hc * 3);									//if I had implemented the circular queue right
													//I wouldn't have to have this much space but 
													//It ended up seg faulting a lot and I decided 
													//to just have it as an array until I can get
													//smart enough to fix this
	
	int i;
	for(i = 0; i < 256; i++)									//for each spot in the histogram, if the count
	{												//is above 0, make a tree node and insert it
		if(histogram[i] > 0)									//into the queue.
		{
			treeNode *temp = newNode((char)i, histogram[i], true); 
			enQ(entries, temp);
		}  	
	}

	treeNode *treehead;										//make a place for the head of the tree to be placed
	makeTree(entries, &treehead);									//pass to the function making a tree with the queue
													//and a pointer to the tree head so that it would store 
													//it there

	if(printt)											//if print tree is true, print tree.
	{
		printTree(treehead, treehead->depth/(2*hc));	
	}


/**================ gathering code & printing tree to file & compressing data =======================**/

	
	bitStack *codelett[256] = {0};									//make and initialize the array of codes to be stored
													//for each byte in the file
	buildCode(treehead, codelett, out);								//pass it to the function which reads in the tree and 
													//stores code while writing to the file and destroying
													//the tree at the same time.
													//will need the file descriptor for output file and the
													//tree, and the array of codes to store this in. 
	
	bitStack *master = newBS(fileStat.st_size * 8);							//master is the bitstack for actually building the compressed
													//bits for the file.
	
	if(!nothing)											//if there are actually things in the file, when the memory
	{												//mapping has not failed, then read in the files.
		readFile(input, master, codelett, fileStat.st_size);
	}


/**=========================== writing compressed bitStack to file ===================================**/
	
	uint32_t u;											//u for looping through the file
	uint32_t size = (master->top)/8+1; 								//size for the limit, since we're approaching each box in the
													//bit vector.		
 
	for(u = 0; u < size; u++)									//for loop to loop through the whole bit vector and write each
	{ 												//bit. the reason why I couldn't push the whole bit vector to
		uint8_t writ = wraparound((master->entries), u);  					//the file is that I store the bits differently (7 being the 
		write(out, &writ, sizeof(writ)); 							//normal position 0, and so on.) I made a function to flip the
													//numbers and print that instead.
	}
	if(verbose)											//if verbose, print all the necessary informations.
	{
		struct stat outStat;							
		if(fstat(out, &outStat) == -1)								//calculate the final output file.
		{
			perror("wrong with reading the file stat for out");				//if it wrote nothing to the output, throw an error. 
			strerror(errno);
			exit(errno);
		}	
	
		if(!nothing)										//this splits if it's a file with nothing in it or not.
		{											//if it read in something, then print the usual information.
			
			float percent = ((float)(master->top) / (fileStat.st_size * 8)) * 100;	
			printf("leaves %d (%u bytes) and encoding %u bits(%f%%)\n", hc, treeCount, (master->top), percent);	
		}
		else											//otherwise, print -nan for the percent and 0 for the master->top
		{											//because it obviously did not read anything in.
			printf("leaves %d (%u bytes) and encoding %d bits(%s %%)\n", hc, treeCount, 0, "-nan");
		}
	}
	
/**=============================================free vars ============================================**/

	delBS(master);											//at this point, i'm just freeing all the memory allocated variables.
	int p;
	for(p = 0; p < 256; p++)
	{
		if(codelett[p])
		{
			delBS(codelett[p]);
		}	
	}
	delQ(entries);
	int cl = close(in);
	int ose = close(out);
	if(cl == -1 || ose == -1)
	{
		perror("Cannot close the files");
		strerror(errno);
		exit(errno);
	}
	munmap(input, fileStat.st_size); 		  
	
	return 0;											//success!
}


/**
 * printTree function.
 *
 * printing the tree if printt is 
 * true on main.
 *
 * given by DDEL on piazza for
 * class 12B intro to data structure
 * in UCSC.
 *
 * @param *t the head of the tree
 * @param depth depth of the tree
 *
 */
void printTree(treeNode *t, int depth)
{
	if (t) 
	{
		printTree(t->left, depth + 1);

		if (t->leaf)
		{
			if (isalnum(t->symbol))
			{
				spaces(4 * depth); printf("'%c' (%llu)\n", t->symbol, t->count);	
			}
			else
			{
			spaces(4 * depth); printf("0x%X (%llu)\n", t->symbol, t->count);
			}
		}
		else
		{
			spaces(4 * depth); printf("$ (%llu)\n", t->count);
		}

		printTree(t->right, depth + 1); 
	}

	return;
}


/**
 * wraparound function.
 *
 * function made last minute because
 * I just realized that everyone stores
 * bit vectors differently than mine.
 *
 * basically wraps around the bits so that
 * it'll be read from right to left instead
 * of left to right.
 *
 * @param v the bit vector 
 * @param ind the index of the number to 
 * flip around
 *
 * @return flipped the data collected from 
 * flipping around the number.  
 */
uint8_t wraparound(bitV *v, uint32_t ind)								
{
	bitV *temp = newVec(7);										//create a new bitStack
	int counter = 7;										//counter to re-position the bits
	while(counter >= 0)										//while it didn't finish
	{
		if(valBit(v, 7-counter + (ind * 8)) == 1)						//see each bit and either
		{											//set the bit if it's 1
			setBit(temp, counter);								//or clear the bit if it's 0
		}
		else
		{
			clrBit(temp, counter);
		}

		counter--;										//decrement counter as we 
	}												//move forward.

	uint8_t flipped = temp->v[0];									//put it in a unsigned 8 bit int
	delVec(temp);											//so wee can free the bit vector
	return flipped;											//and return the number.

	

}

/**
 * readFile function.
 *
 * reads the memory mapped contents
 * of the file and appends to the
 * bitStack called master.
 *
 * @param file the memory mapped file
 * @param master the bitStack being appended
 * @param codelett the stored codes for each 
 * character
 * @param size the size of the file.
 *
 */
void readFile(uint8_t *file, bitStack *master, bitStack *codelett[256], unsigned long size)
{
	unsigned long i;										
	for(i = 0; i < size; i++)									//a for loop to loop through position
	{												//0 to the end of the file
		   	
		bitStack *bits = codelett[file[i]];							//go to the code of the specific character
	
		uint32_t top = bits->top;								//need to reuse the code later, so if I just
													//save the top, I'll be able to read the same
													//characters over and over again. 

		while(!emptyBS(bits))									//while bits is not empty, copy the code over
		{											//and append the master bitVector.
			pushBS(master, popBS(bits));
		}

		bits->top = top;									//set the top equal to the top again.
		
	}

	return;
} 


/**
 * makeTree function.
 *
 * making the tree from a queue
 * and storing the tree head in a position 
 * in the main function.
 *
 * @param q the queue
 * @param treehead the place to store
 * the head of the tree.
 *
 */
void makeTree(queue *q, treeNode **treehead)
{
	while(!oneLeft(q))										//while the queue has two or more nodes
	{
		insertionSort(q, q->head, q->tail);							//run the insertion sort on the queue
		treeNode *one;										//pop two and store them accordingly.
		deQ(q, &one);
		treeNode *two;
		deQ(q, &two);
		treeNode *three;									//creating a third to join the two
		if(one->count <= two->count)								//making sure that the bigger tree node
		{											//gets stored on the right and not on the left.
			join(one, two, &three);
		}
		else if(two->count > one->count)
		{
			join(two, one, &three);
		}

		enQ(q, three);										//push the third joined node back in.
	}
	
	deQ(q, treehead);										//pop the head of the tree off when there's
													//only one node left.
	return;
}  


/**
 * histoCount function.
 *
 * counts how many characters there are
 * in the input text file and stores
 * it in the array of unsigned long ints.
 *
 * @param file the memory mapped input file pointer
 * @param size the size of the file
 * @param histogram the histogram to store counts
 * @param hc the number of leaves on the tree. 
 *
 */
void histoCount(uint8_t *file, unsigned long size, uint64_t histogram[256], int *hc)
{

	if(!nothing)											//if there is something to read in the file
	{
		unsigned long i;
		for(i = 0; i < size; i++)								//increment each time there's a character in
		{											//the file for counting.
			histogram[file[i]] += 1; 
		}

		int l;
		for(l = 0; l < 256; l++)								//count how many slots are filled up.
		{
			if(histogram[l] > 0)
			{
				(*hc)++;	
			}
		}
	}

	if(fulltree)											//if the user requires a full tree, then
	{
		int i;
		for(i = 0; i < 256; i++)								//for loop for incrementing every other letter
		{
			if(histogram[i] < 1)								//that has not been incremented to add to the 
			{										//queue and make the tree a full one.  
				histogram[i]++;
				(*hc)++;
			}
		}
	}
	else												//otherwise, only increment the 0th character 
	{												//and 255 for avoiding the case where there is no
													//tree beacuse the file contains nothing.
		if(histogram[0] < 1)
		{
			histogram[0] += 1;
			(*hc)+=1;		
		}
		if(histogram[255]<1)
		{
			histogram[255] += 1;
			(*hc)+=1;
		}
	}

	return;
}
