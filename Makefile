CC=gcc
DE_OBJECTS= decode.o huffmanJN.o stack.o
EN_OBJECTS= bitStack.o bv.o encode.o huffman.o insertions.o queue.o treeNodeStack.o
CFLAGS=  -Wall -Wextra -Wpedantic -Werror -g

.PHONY : all
all : encode decode

bitStack.o : bitStack.c
	$(CC) -c bitStack.c

bv.o : bv.c
	$(CC) -c bv.c

decode : $(DE_OBJECTS)
	$(CC) $(CFLAGS) $(DE_OBJECTS) -o decode

encode : $(EN_OBJECTS)
	$(CC) $(CFLAGS) $(EN_OBJECTS) -o encode

decode.o : decode.c
	$(CC) $(CFLAGS) -c decode.c

encode.o : encode.c
	$(CC) $(CFLAGS) -c encode.c

huffman.o : huffman.c
	$(CC) $(CFLAGS) -c huffman.c

huffmanJN.o : huffmanJN.c 
	$(CC) $(CFLAGS) -c huffmanJN.c

insertions.o : insertions.c
	$(CC) -c insertions.c

queue.o : queue.c
	$(CC) -c queue.c

stack.o : stack.c
	$(CC) $(CFLAGS) -c stack.c

treeNodeStack.o : treeNodeStack.c
	$(CC) $(CFLAGS) -c treeNodeStack.c

.PHONY : clean
clean : 
	rm -f decode encode $(DE_OBJECTS) $(EN_OBJECTS)
