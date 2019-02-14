/**
 * Yu Jung
 *
 * huffman.c
 */

# include "huffman.h"


/**
 * buildCode function.
 *
 * so.
 *
 * this is the most complicated
 * function that I've written for
 * this project.
 *
 * I knew I could have taken the 
 * recursive way out faster but I 
 * planned this out on paper and wrote
 * all this code before the TA's taught
 * me that I could do this recursively.
 *
 * I know that it looks really complicated but
 * it really isn't when you see it because
 * it's really just accounting for every single
 * case while going through the tree.
 *
 * it:
 * 	1)stores the code while encountering a leaf
 *	2)prints to the output the whole tree
 *	3)deletes the whole tree 
 *
 * while post-traversing the tree.
 *
 * look. I'm a simple person
 * if I wrote this, and explained it, 
 * it probably isn't that hard to get.
 *
 *
 * @param head the head of the tree
 * @param codeletts the array to store the code
 * @param out the file descriptor for writing
 * to the output file.
 *
 */
void buildCode(treeNode *head, bitStack *codeletts[256], int out)
{
	bitStack *follow = newBS(256);						//this bitStack will be used in storing
										//all the code while following the tree.
	treeNode *current = head;						//set the current at the head
	treeNode *daddy;							//we need this node to store the parent.
	bool first = true;							//this bool is needed because while 
										//traversing the tree, a variable is needed
										//to see if we have gone to the right 
										//side of the tree at all.				

				   /** ==================================traveling to the first leaf on left ===========*/	

	while(current->left != NULL)						//we first travel to the left most leaf
	{									//of the tree.
		current = current->left;
		pushBS(follow, 0);	
	} 
	
				  /** ===================================================================================*/



	while(current != head)
	{
		daddy = areYouMyDad(follow, head);				//first calculate the parent.

		if(!current->leaf) /** =================================== if current is an internal node ==============*/
		{
			if(current->left)					//check if it has either of the children.
			{
				pushBS(follow, 0);				//if it does, then enter the loop again
				current = current->left;			//with that child.
			}	
			else if(current->right)
			{
				pushBS(follow,1);
				current = current->right;
			}

			else if(!current->left&&!current->right)		//if it does not have any children,
			{
				char i = 'I';				
				write(out, &i, sizeof(i));			//print an I to the output file,
				uint8_t temp = popBS(follow);			//pop one on the code stack,
				if(temp == 0)					//check which one it is and set
				{		
					daddy->left = NULL;			//whichever path to NULL so we don't
				}						//free the same thing twice later
				else
				{
					daddy->right = NULL;
				}
				delNode(current);				//delete the node,
				current = daddy;				//travel to the parent.
			}
		}		/** ==================================================================================*/








		else		/** ======================================if the current node is a leaf =============*/
		{
			char wr = 'L';
			uint8_t sym = current->symbol;  
			write(out, &wr, sizeof(wr));				//write to the output and note
			write(out, &sym, sizeof(sym));				//the symbol too.
	
			copyCode(&(codeletts[(int)current->symbol]), follow); 	//copy code onto the array of codes.
			
			uint8_t bit = popBS(follow);				//pop a code
			if(bit == 0)						//set the right path to NULL on
			{							//the parent
				daddy->left = NULL;
			}
			else
			{
				daddy->right = NULL;
			}
			delNode(current);					//delete the current node
			current = daddy;					//travel back to the parent.

		}		/** ================================================================================*/



		if(current == head)						//if we've reached the end
		{
			if(first)						//if we haven't gone to the right side
			{							//of the head
				pushBS(follow, 1);				//push a 1
				current = current->right;			//travel to the right
				first = false;					//set the trigger bool to false.
			}	
		}	
	}
	
	char last = 'I';							//lastly, print out an extra I for 
	write(out, &last, sizeof(last));					//the head.
	delNode(head);								//delete the head 
	delBS(follow);								//delete the code stack following
		
	return;									//poof! done
}



