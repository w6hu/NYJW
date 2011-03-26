#include "priority.h"

void init_pm (struct PCB* pcb_pm, UINT32* stackPtr)
{	
	pcb_pm->next = NULL;
	pcb_pm->id = SET_PRIORITY_ID;
	pcb_pm->priority = 0;
	pcb_pm->stack = stackPtr;
	pcb_pm->returning = FALSE;
	pcb_pm->waiting_on = -1;
		
	int val;
	//back up a7
	int original_a7;
	asm("move.l %%a7, %0" : "=r" (original_a7));	
	val = pcb_pm->stack;
	asm("move.l %0, %%a7" : : "r" (val));
	val = priority_modifier;			
	asm("move.l %0, %%d0" : : "r" (val));
	asm("move.l %d0, -(%a7)");
	val = 4;			
	asm("move.w %0, %%d0" : : "r" (val));
	asm("move.w %d0, -(%a7)");
	val = 16512;			
	asm("move.w %0, %%d0" : : "r" (val));
	asm("move.w %d0, -(%a7)");
	pcb_pm->stack -= 8;

	//restore a7
	asm("move.l %0, %%a7" : : "r" (original_a7));
		
	// initialize the process to the correct ready queue
	put_to_ready(pcb_pm);
	pcb_pm->state = STATE_NEW;	
}

void priority_modifier()
{
	void* error_message;
	// the first thing it does it register itself
	register_command(SET_PRIORITY_ID, 'C');
	
	while(1)
	{
		int sender;
		void* user_command = receive_message(&sender);
					
		int process_id;
		int new_priority;
		int command_length = *((int *)user_command + 16);
		if(command_length != 5)
			goto PRIORITY_CHECK_ERROR; 
		if( *((char *)user_command + 68) != ' ')
			goto PRIORITY_CHECK_ERROR;
		
		process_id = *((char *)user_command + 69)-48;
		if( process_id > 9 || process_id < 1)
			goto PRIORITY_CHECK_ERROR;

		if( *((char *)user_command + 70) != ' ')
			goto PRIORITY_CHECK_ERROR;	

		new_priority = *((char *)user_command + 71)-48;
		if( new_priority > 3 || new_priority < 0)
			goto PRIORITY_CHECK_ERROR;			
		
		if( *((char *)user_command + 72) != CR)
			goto PRIORITY_CHECK_ERROR;	
		
		set_process_priority(process_id, new_priority);
		
		goto PRIORITY_END_CHECK;
	
	PRIORITY_CHECK_ERROR:
		error_message = request_memory_block();
		*((int *)error_message) = COMMAND_ERROR;
		send_message(CRT_ID, error_message);
	
	PRIORITY_END_CHECK:	
		release_memory_block(user_command);
	}
}