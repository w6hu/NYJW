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
	asm("move.l %d0, -(%a7)");
	asm("move.l %0, %d0" : : "r" (process_ID));
	asm("move.l %d0, -(%a7)");	
	asm("move.l %0, %d0" : : "r" (MessageEnvelope));
	asm("move.l %d0, -(%a7)");	
	
	int val = CALLER_SEND_MESSAGE;
	asm("move.l %0, %%d0" : : "r" (val));
	asm( "TRAP #0" );

	// gets the return value
	int return_val;
	asm("move.l (%a7)+ ,%d0");
	asm("move.l %d0, %0" : "=r" (return_val));	
	
	asm("move.l (%a7)+ ,%d0");
	asm("move.l (%a7)+ ,%d0");	
	
    //rtx_dbug_outs((CHAR *)"rtx: send_message \r\n");
	return return_val;
}

void * receive_message(int * sender_ID)
{
	asm("move.l %d0, -(%a7)");
	asm("move.l %0, %d0" : : "r" (sender_ID));
	asm("move.l %d0, -(%a7)");	
	asm("move.l #1, %d0"); // pass in an extra parameter, user who call this function have no option but
						   // to be blocked; a 1 indicate that it should be blocked if no message.
	asm("move.l %d0, -(%a7)");
	
	int val = CALLER_RECEIVE_MESSAGE;
	asm("move.l %0, %%d0" : : "r" (val));
	asm( "TRAP #0" );	
	
	int return_val;
	asm("move.l (%a7)+ ,%d0");
	asm("move.l %d0, %0" : "=r" (return_val));		
	
	asm("move.l (%a7)+ ,%d0");	
	asm("move.l (%a7)+ ,%d0");
    //rtx_dbug_outs((CHAR *)"rtx: receive_message \r\n");
	return return_val;
}

/*Memory Management*/
void * request_memory_block() 
{
	asm("move.l %d0, -(%a7)");
	asm("move.l %d0, -(%a7)");	

	int val = CALLER_REQUEST_MEMORY_BLOCK;
	asm("move.l %0, %%d0" : : "r" (val));
	asm( "TRAP #0" );
	
	// gets the return value
	int return_val;
	asm("move.l (%a7)+ ,%d0");
	asm("move.l %d0, %0" : "=r" (return_val));
	
	asm("move.l (%a7)+ ,%d0");


    //rtx_dbug_outs((CHAR *)"rtx: request_memory_block \r\n");
	return (void*)return_val; 
}

int release_memory_block(void * MemoryBlock)
{
	asm("move.l %d0, -(%a7)");
	asm("move.l %0, %d0" : : "r" (MemoryBlock));
	asm("move.l %d0, -(%a7)");	

	int val = CALLER_RELEASE_MEMORY_BLOCK;
	asm("move.l %0, %%d0" : : "r" (val));
	asm( "TRAP #0" );
	
	// gets the return value
	int return_val;
	asm("move.l (%a7)+ ,%d0");
	asm("move.l %d0, %0" : "=r" (return_val));
	
	asm("move.l (%a7)+ ,%d0");


    //rtx_dbug_outs((CHAR *)"rtx: release_memory_block \r\n");
	return return_val;
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
	asm("move.l %d0, -(%a7)");
	asm("move.l %0, %d0" : : "r" (process_ID));
	asm("move.l %d0, -(%a7)");	
	asm("move.l %0, %d0" : : "r" (MessageEnvelope));
	asm("move.l %d0, -(%a7)");	
	asm("move.l %0, %d0" : : "r" (delay));
	asm("move.l %d0, -(%a7)");	
	
	int val = CALLER_DELAYED_SEND;
	asm("move.l %0, %%d0" : : "r" (val));
	asm( "TRAP #0" );

	// gets the return value
	int return_val;
	asm("move.l (%a7)+ ,%d0");
	asm("move.l %d0, %0" : "=r" (return_val));	
	
	asm("move.l (%a7)+ ,%d0");
	asm("move.l (%a7)+ ,%d0");	
	asm("move.l (%a7)+ ,%d0");
	
    //rtx_dbug_outs((CHAR *)"rtx: send_message \r\n");
	return return_val;
}

/*Process Priority*/
int set_process_priority(int process_ID, int priority)
{
	asm("move.l %d0, -(%a7)");
	asm("move.l %0, %d0" : : "r" (process_ID));
	asm("move.l %d0, -(%a7)");	
	asm("move.l %0, %d0" : : "r" (priority));
	asm("move.l %d0, -(%a7)");	
	
	int val = CALLER_SET_PRIORITY;
	asm("move.l %0, %%d0" : : "r" (val));
	asm( "TRAP #0" );

	// gets the return value
	int return_val;
	asm("move.l (%a7)+ ,%d0");
	asm("move.l %d0, %0" : "=r" (return_val));	
	
	asm("move.l (%a7)+ ,%d0");
	asm("move.l (%a7)+ ,%d0");	
	
    //rtx_dbug_outs((CHAR *)"rtx: send_message \r\n");
	return return_val;
}

int get_process_priority(int process_ID)
{
	asm("move.l %d0, -(%a7)");
	asm("move.l %0, %d0" : : "r" (process_ID));
	asm("move.l %d0, -(%a7)");	

	int val = CALLER_GET_PRIORITY;
	asm("move.l %0, %%d0" : : "r" (val));
	asm( "TRAP #0" );
	
	// gets the return value
	int return_val;
	asm("move.l (%a7)+ ,%d0");
	asm("move.l %d0, %0" : "=r" (return_val));
	
	asm("move.l (%a7)+ ,%d0");


    //rtx_dbug_outs((CHAR *)"rtx: release_memory_block \r\n");
	return return_val;
}
