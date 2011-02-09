/**
 * @author: 
 * @brief: ECE 354 S10 RTX Project P1-(c) 
 * @date: 2011/01/07
 */

#include "rtx_inc.h"
#include "memory.h"

extern UINT32 __end;
extern UINT32 *free_blocks;
extern UINT32 *used_blocks;

const BYTE USED = 1;
const BYTE FREE = 0;
const BYTE SUCCESS = 0;
const BYTE FAILURE = 1;

void* s_request_memory_block()
{
	if(*free_blocks == NULL)	
		return NULL;
		
	UINT32	*allocated_block;
	allocated_block = free_blocks;
	*(allocated_block+2) = USED;
	
	free_blocks = *free_blocks;
	*(free_blocks+1) = NULL;
	return (void*)(allocated_block+3);
}

/**
 * @param: address of a memory block
 * @return: 0 on sucess, non-zero otherwise
 */
int s_release_memory_block( void* memory_block )
{
	UINT32 *memory_address;
	memory_address = (UINT32 *)memory_block - 3;
	memory_block = memory_address;
	UINT32 next = (*(UINT32 *)memory_block);
	UINT32 previous = *((UINT32 *)memory_block+1);
	//UINT32 next = *memory_block;
	//UINT32 previous = *(memory_block+1);
	int status = *((int *)memory_block+2);
	if (status != USED) {
		return FAILURE;
	}
	if (next) {
		*((UINT32 *)next+1) = previous;
	}
	if (previous) {
		*((UINT32 *)previous) = next;
	}
	if (*free_blocks != NULL) {
		*(free_blocks + 1) = memory_block;
	}
	*((UINT32 *)memory_block) = free_blocks;
	*((UINT32 *)memory_block + 1) = 0;
	free_blocks = memory_block;
	*((int *)memory_block+2) = FREE;
	return SUCCESS;
}


void *initBlock(int num_blocks)
{
	UINT32 *temp_end = &__end; 
	int i=0;
	
	*(temp_end+1) = NULL;
	*(temp_end+2) = FREE;
	
	for(i; i<num_blocks; i++)
	{
		UINT32	*prev = temp_end; 
		*temp_end = temp_end + 140/4;

		temp_end = temp_end + 140/4;
		*(temp_end+1) = prev;
		*(temp_end+2) = FREE;
	}
	
	*temp_end = NULL;
	
	return &__end;
}

		/*
		int last; //= tempEnd%10;
		int remain = *temp_end;
		//int i = 0; 
		while (remain != 0) {
			//rtx_dbug_out_char((CHAR)(last+48));
			last = remain%10;
			remain = remain/10;
			rtx_dbug_out_char((CHAR)(last+48));            
		}
		rtx_dbug_outs((CHAR *) "\r\n");
		*/
