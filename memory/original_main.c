/**
 * @author: 
 * @brief: ECE 354 S10 RTX Project P1-(c) 
 * @date: 2010/05/03
 * This file runs simiple tests of memory allocation/de-allocation APIs.
 * During the demo, we may replace this file with one  
 * that contains more testing cases. 
 */

#include "rtx_inc.h"
#include "dbug.h"
#include "memory.h"

#define NUM_MEM_BLKS 32

UINT32 *free_blocks;
UINT32 *used_blocks = NULL;

/* gcc expects this function to exist */
int __main( VOID )
{
    return 0;
}


int main( VOID )
{
    int i,j;
    void* p_mem_array[NUM_MEM_BLKS+1];
	
	free_blocks = initBlock(NUM_MEM_BLKS);
	
    for ( i=0; i< NUM_MEM_BLKS; i++ ) 
    {
        p_mem_array[i] = s_request_memory_block();
			
		//rtx_dbug_out_char((CHAR)(i+48));
        if (p_mem_array[i] == NULL) {
            rtx_dbug_outs((CHAR *) "Null pointer.\r\n");
        } else if (p_mem_array[i] > 0x10200000) {
            rtx_dbug_outs((CHAR *) "Memory out of bound. \r\n");
        } else {
            rtx_dbug_outs((CHAR *) "Request meory block: almost OK\r\n");
        }
		
		
		/*
		int last;
		int remain = p_mem_array[i];
		//int i = 0; 
		while (remain != 0) {
			//rtx_dbug_out_char((CHAR)(last+48));
			last = remain%10;
			remain = remain/10;
			rtx_dbug_out_char((CHAR)(last+48));            
		}
		rtx_dbug_outs((CHAR *) "\r\n");
		
		p_mem_array[i]++;
		
		last;
		remain = p_mem_array[i];
		//int i = 0; 
		while (remain != 0) {
			//rtx_dbug_out_char((CHAR)(last+48));
			last = remain%10;
			remain = remain/10;
			rtx_dbug_out_char((CHAR)(last+48));            
		}
		rtx_dbug_outs((CHAR *) "\r\n");
		rtx_dbug_outs((CHAR *) "\r\n");
		*/
		
		for(j=0; j<128/4; j++)
		{
			*((UINT32* )p_mem_array[i]+j) = (UINT32)0;
		}
    }

    for ( i=0; i< NUM_MEM_BLKS; i++ ) 
    {
        int temp;
        temp = s_release_memory_block( p_mem_array[i] );
        if (temp == 0 ) {
            rtx_dbug_outs((CHAR *) "Release memory block: OK\r\n");
        } else {
            rtx_dbug_outs((CHAR *) "Release memory block: Failed \r\n");
        }
    }
  
	/*rtx_dbug_outs((CHAR *) "\r\n");
	for ( i=0; i< NUM_MEM_BLKS; i++ ) 
    {
        int temp;
        temp = s_release_memory_block( p_mem_array[i] );
        if (temp == 0 ) {
            rtx_dbug_outs((CHAR *) "Release memory block: OK\r\n");
        } else {
            rtx_dbug_outs((CHAR *) "Release memory block: Failed \r\n");
        }
    }
	
    for ( i=0; i< NUM_MEM_BLKS+1; i++ ) 
    {
        p_mem_array[i] = s_request_memory_block();
		
		//rtx_dbug_out_char((CHAR)(i+48));
        if (p_mem_array[i] == NULL) {
            rtx_dbug_outs((CHAR *) "Null pointer.\r\n");
        } else if (p_mem_array[i] > 0x10200000) {
            rtx_dbug_outs((CHAR *) "Memory out of bound. \r\n");
        } else {
            rtx_dbug_outs((CHAR *) "Request meory block: almost OK\r\n");
        }
    }
*/
    return 0;
}
