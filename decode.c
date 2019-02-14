// John Nover
// decode.c
// June 7th, 2017
// CMPS 12B

# include "decode.h"

// Very helpful in debugging

static inline void spaces(int c)
{
	for (int i = 0; i < c; i += 1)
	{
		putchar(' ');
	} 
	return;
}

void printTree(treeNode *t, int depth)
{
	if (t)
	{
		printTree(t->left, depth + 1);
		if(t->leaf)
		{
			if(isalnum(t->symbol))
			{
				spaces(4 * depth); 
				printf("'%c' (%d)\n", t->symbol, t->leaf);
			}
			else
			{
				spaces(4 * depth);
				printf("0x%X (%d)\n", t->symbol, t->leaf);
			}
		}
		else
		{
			spaces(4 * depth); printf("$ (%lu)\n", t->count);
		}
		printTree(t->right, depth + 1);
	}
	return;
}

treeNode *parseTree(int64_t *file, uint16_t length)
{
	uint8_t symbol;
	uint8_t leafSymbol = 'L';
	uint8_t joinSymbol = 'I';
	stack *treeStack = newStack(length);
	treeNode *root;
	for(uint16_t i = 0; i < length; i++)
	{
		read(*file, &symbol, 1);
		if(memcmp(&symbol, &leafSymbol, 1) == 0) 
		{
			read(*file, &symbol, 1);
			i++;
			treeNode *x = newNode(symbol, 1, 0);
			push(treeStack, x);
		} else if(memcmp(&symbol, &joinSymbol, 1) == 0)
		{
			treeNode *right = pop(treeStack);
			treeNode *left = pop(treeStack);
			root = join(left, right);
			push(treeStack, root);
		} 
	}
	delStack(treeStack);
	return root; 
}

void decodeFile(int64_t *inFile, int64_t *outFile, treeNode *tree, uint64_t fileSize)
{
	uint64_t bytesWritten = 0;
	uint8_t i = 0, bit = 0, byte = 0;
	treeNode *root = tree, *climber = root;

	while(bytesWritten < fileSize)
	{
		if(read(*inFile, &byte, 1) == -1) break;
		i = 0;
		while(i < 8)
		{
			if(bytesWritten == fileSize) {break;}
			
			// This gets the next bit at pos [i] and then
			// increments the position counter by 1.

			bit = ((byte >> i) & 1);
			i++;

			// While I could have used a stepTree function,
			// I thought it was nicer if I just wrote it in
			// here without the extra function call.

			if(bit == 0) climber = climber->left;
			if(bit == 1) climber = climber->right;
			
			if(climber->leaf)
			{
				write(*outFile, &climber->symbol, 1);
				bytesWritten += 1;
				climber = root;
			}
		}
	}
	return;
}

bool checkFile(int64_t *file) // Function to test if a file contains key
{
	uint32_t key = 0xdeadd00d; // 4 bytes
	uint32_t buff; // 4 byte char buffer

	read(*file, &buff, 4); //Read from file to buff

	if(memcmp(&buff, &key, 4) == 0) // Compare key with buffer
	{
		return true; 
	}
	return false;
}

uint64_t fileLength(int64_t *file)
{
	uint64_t size = 0;
	read(*file, &size, 8); // Read from buffer
	return size;	
}

uint16_t treeLength(int64_t *file)
{
	uint16_t size = 0;
	read(*file, &size, 2); // ditto ^
	return size;
}

int main(int argc, char *argv[])
{
	if(argc == 1) 
	{
		fprintf(stderr, "Not a compressed file\n");
		return 1;
	}
	int option;
	bool verbose = false, print = false;
	char *iFile, *oFile = NULL;

	while ((option = getopt(argc, argv, "i:o:vp")) != -1)
	{
		switch(option)
		{
			case 'i' : { iFile = optarg; break; }
			case 'o' : { oFile = optarg; break; }
			case 'v' : { verbose = true; break; }
			case 'p' : { print = true; break; }
		}
	}
	int64_t fdIn = open(iFile, O_RDONLY);
	int64_t fdOut;

	if(checkFile(&fdIn) == false)
	{
		fprintf(stderr, "Not a compressed file\n");
		return 0;
	}

	if(oFile != NULL)
	{
		if((fdOut = open(oFile, O_RDWR | O_CREAT | O_EXCL, 0644)) == -1)
		{
			fprintf(stderr, "Output file already exists!\n");
			return 0;
		}
	}
	else
	{
		fdOut = 1;
	}
	
	uint64_t fileSize = fileLength(&fdIn);
	uint16_t treeSize = treeLength(&fdIn);

	if(verbose)
	{
		printf("Original %lu bits: ", fileSize * 8);
		printf("tree (%d)\n", treeSize);
	}

	treeNode *tree = parseTree(&fdIn, treeSize);
	
	if(print) printTree(tree, 0);

	decodeFile(&fdIn, &fdOut, tree, fileSize);

	delTree(tree);
	
	return 0;
}
