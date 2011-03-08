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
	
	init_mailboxes();
	
	void* msg1 = s_request_memory_block();
	void* msg2 = s_request_memory_block();
	
	rtx_dbug_outs("send message");
	
	*((UINT32 *)msg1 + 10) = 2;
	*((UINT32 *)msg1 + 11) = 3;
	*((UINT32 *)msg2 + 10) = 4;
	*((UINT32 *)msg2 + 11) = 5;
	
	int result = send_message(2, msg1);
	int result2 = send_message(2, msg2);
	rtx_dbug_out_char(result + 48);
	rtx_dbug_out_char('\r');
	rtx_dbug_out_char('\n');
	rtx_dbug_out_char(result2 + 48);
	rtx_dbug_out_char('\r');
	rtx_dbug_out_char('\n');
	
	rtx_dbug_outs("receive message");
	
	int val  = 1;
	int *sender_id = &val; 
	void* reMsg1 = receive_message(sender_id);
	if (reMsg1 != NULL) {
		rtx_dbug_outs("SUCCESS!!\r\n");
		rtx_dbug_out_char(*((UINT32 *)reMsg1 + 10)+48);
		rtx_dbug_out_char(*((UINT32 *)reMsg1 + 11)+48);
	}
	
	void* reMsg2 = receive_message(sender_id);
	if (reMsg2 != NULL) {
		rtx_dbug_outs("SUCCESS!!\r\n");
		rtx_dbug_out_char(*((UINT32 *)reMsg2 + 10)+48);
		rtx_dbug_out_char(*((UINT32 *)reMsg2 + 11)+48);
	}
	else {
		rtx_dbug_outs("No message!!\r\n");
	}
	
	void* reMsg3 = receive_message(sender_id);
	if (reMsg3 != NULL) {
		rtx_dbug_outs("SUCCESS!!\r\n");
		rtx_dbug_out_char(*((UINT32 *)reMsg3 + 10)+48);
		rtx_dbug_out_char(*((UINT32 *)reMsg3 + 11)+48);
	}
	else {
		rtx_dbug_outs("No message!!\r\n");
	}
	
    return 0;
}
