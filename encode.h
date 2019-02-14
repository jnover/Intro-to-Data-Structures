/**
 * Yu Jung
 *
 * encode.h
 */

# include <stdlib.h>
# include <getopt.h>
# include <string.h>
# include <stdio.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <unistd.h>
# include <fcntl.h>
# include <math.h>

# include "bitStack.h"  
# include "treeNodeStack.h"
# include "queue.h"
# include "insertions.h"

void histoCount(uint8_t *, unsigned long, uint64_t *, int *);
void makeTree(queue *, treeNode **);
void readFile(uint8_t *, bitStack *, bitStack *[], unsigned long);
uint8_t wraparound(bitV *, uint32_t ind);
void printTree(treeNode *, int);

//spaces function, which puts the spaces in for printing the tree.
//given by DDEL in class 12B, intro to data structures in UCSC.
static inline void spaces(int c) { for (int i = 0; i < c; i += 1) { putchar(' '); } return; }
