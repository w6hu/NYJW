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
#include "messaging.h"

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
	
	free_blocks = initBlock(NUM_MEM_BLKS);
	
	void* msg1 = s_request_memory_block();
	int result = send_message(2, msg1);
	rtx_dbug_out_char(result + 48);
	
	
    return 0;
}
