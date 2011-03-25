/**
 * @author: 
 * @brief: ECE 354 S10 RTX Project P1-(c)
 * @date: 2011/01/07
 */

 #include "rtx_inc.h"
 #include "dbug.h"
 
UINT32* mem_end; 
 
/* Prototypes */
void* s_request_memory_block_yishi(); 
int s_release_memory_block_yishi( void* memory_block );
void *initBlock(int num_blocks);
