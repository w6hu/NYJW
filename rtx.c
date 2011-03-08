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
	return send_message_jessie (process_ID, MessageEnvelope);
}

void * receive_message(int * sender_ID)
{
	rtx_dbug_outs((CHAR *)"rtx: receive_message \r\n");
	int last; //= tempEnd%10;
	int remain = sender_ID;
	//int i = 0; 
	while (remain != 0) {
		//rtx_dbug_out_char((CHAR)(last+48));
		last = remain%10;
		remain = remain/10;
		rtx_dbug_out_char((CHAR)(last+48));            
	}
	rtx_dbug_outs((CHAR *) "\r\n");
    rtx_dbug_outs((CHAR *)"rtx: receive_message \r\n");
	return receive_message_jessie(sender_ID);
}

/*Memory Management*/
void * request_memory_block() 
{

    rtx_dbug_outs((CHAR *)"rtx: request_memory_block \r\n");
	void* block = s_request_memory_block_yishi();
	int last; //= tempEnd%10;
	int remain = block;
	//int i = 0; 
	while (remain != 0) {
		//rtx_dbug_out_char((CHAR)(last+48));
		last = remain%10;
		remain = remain/10;
		rtx_dbug_out_char((CHAR)(last+48));            
	}
	rtx_dbug_outs((CHAR *) "\r\n");
	return block; 
}

int release_memory_block(void * MemoryBlock)
{
    rtx_dbug_outs((CHAR *)"rtx: release_memory_block \r\n");
	return s_release_memory_block_yishi(MemoryBlock);
}

/*Process Management*/
int release_processor()
{
    rtx_dbug_outs((CHAR *)"rtx: release_processor result ");
	int val = release_processor_kuma_san();
    rtx_dbug_out_char((CHAR) val +48);
    rtx_dbug_outs((CHAR *)"\r\n");
	return val;
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
    return get_process_priority_usagi_san(process_ID);
}
