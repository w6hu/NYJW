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
    rtx_dbug_outs((CHAR *)"rtx: send_message \r\n");
	send_message_jessie (process_ID, MessageEnvelope);
    return 0;
}

void * receive_message(int * sender_ID)
{
    rtx_dbug_outs((CHAR *)"rtx: receive_message \r\n");
	receive_message_jessie(sender_ID);
}

/*Memory Management*/
void * request_memory_block() 
{
    rtx_dbug_outs((CHAR *)"rtx: request_memory_block \r\n");
	request_memory_block_yishi(); 
    return NULL;
}

int release_memory_block(void * MemoryBlock)
{
    rtx_dbug_outs((CHAR *)"rtx: release_memory_block \r\n");
	release_memory_block_yishi(MemoryBlock)
    return 0;
}

/*Process Management*/
int release_processor()
{
    rtx_dbug_outs((CHAR *)"rtx: release_processor \r\n");
    return release_processor_kuma_san();
}

/*Timing Service*/
int delayed_send(int process_ID, void * MessageEnvelope, int delay)
{
    rtx_dbug_outs((CHAR *)"rtx: delayed_send \r\n");
    return 0;
}

/*Process Priority*/
int set_process_priority (int process_ID, int priority)
{
    rtx_dbug_outs((CHAR *)"rtx: set_process_priority \r\n");
    return 0;
}

int get_process_priority (int process_ID)
{
    rtx_dbug_outs((CHAR *)"rtx: get_process_priority \r\n");
    return 0;
}
