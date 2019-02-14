# include <ctype.h>
# include <errno.h>
# include <fcntl.h>
# include <getopt.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include "stack.h"
# include "huffmanJN.h"

void printTree (treeNode *, int);

treeNode *parseTree (int64_t *, uint16_t);

void decodeFile (int64_t *, int64_t *, treeNode *, uint64_t);

bool checkFile (int64_t *);

uint64_t fileLength (int64_t *);

uint16_t treeLength (int64_t *);
