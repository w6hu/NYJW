/*--------------------------------------------------------------------------
 *                      RTX Stub 
 *--------------------------------------------------------------------------
 */
/**
 * @file:   rtx.c   
 * @author: Irene Huang
 * @date:   2010.05.15
 * @brief:  Dummy rtx primitive stub implementation 
 */

#include "rtx.h"

/* Interprocess Communications*/
int send_message (int process_ID, void * MessageEnvelope)
{
    //rtx_dbug_outs((CHAR *)"rtx: send_message \r\n");
	return send_message_jessie (process_ID, MessageEnvelope);
}

void * receive_message(int * sender_ID)
{
    //rtx_dbug_outs((CHAR *)"rtx: receive_message \r\n");
	return receive_message_jessie(sender_ID);
}

/*Memory Management*/
void * request_memory_block() 
{
    //rtx_dbug_outs((CHAR *)"rtx: request_memory_block \r\n");
	return s_request_memory_block_yishi(); 
}

int release_memory_block(void * MemoryBlock)
{
    //rtx_dbug_outs((CHAR *)"rtx: release_memory_block \r\n");
	return s_release_memory_block_yishi(MemoryBlock);
}

/*Process Management*/
int release_processor()
{
    //rtx_dbug_outs((CHAR *)"rtx: release_processor \r\n");
	int val = CALLER_RELEASE_PROCESSOR;
	asm("move.l %0, %%d0" : : "r" (val));
	asm( "TRAP #0" );
	
    return;
}

/*Timing Service*/
int delayed_send(int process_ID, void * MessageEnvelope, int delay)
{
    //rtx_dbug_outs((CHAR *)"rtx: delayed_send \r\n");
    return 0;
}

/*Process Priority*/
int set_process_priority (int process_ID, int priority)
{
    //rtx_dbug_outs((CHAR *)"rtx: set_process_priority \r\n");
    return 0;
}

int get_process_priority (int process_ID)
{
    //rtx_dbug_outs((CHAR *)"rtx: get_process_priority \r\n");
    return get_process_priority_usagi_san(process_ID);
}
