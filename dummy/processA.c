 #include "processA.h"
 
void process_a() {
	void* block = request_memory_block();
	register_command(PROCESS_A, 'Z'); //Should I be sending a message to KCD to reguster command instead?
	while (TRUE) {
		int sender_id;
		void* message = receive_message(&sender_id);
		if (sender_id == KCD_ID && *((char *)message + 68) != 'Z') {
			release_memory_block(message);
			break;
		} else {
			release_memory_block(message);
		}
	}
	
	int num = 0;
	while (TRUE) {
		void* message = request_memory_block();
		*((UINT32 *)message) = COMMAND_COUNT_REPORT;
		*((UINT32 *)message + 17) =  num;
		send_message(PROCESS_B, message);
		num = num + 1;
		release_processor();
	}	
}

void init_process_a (struct PCB* pcb_proc_a, UINT32* stackPtr) {
	pcb_proc_a->next = NULL;
	pcb_proc_a->id = PROCESS_A;
	pcb_proc_a->priority = 0;
	pcb_proc_a->stack = stackPtr;
	pcb_proc_a->returning = FALSE;
	pcb_proc_a->waiting_on = -1;
		
	int val;
	//back up a7
	int original_a7;
	asm("move.l %%a7, %0" : "=r" (original_a7));	
	val = pcb_proc_a->stack;
	asm("move.l %0, %%a7" : : "r" (val));
	val = process_a;			
	asm("move.l %0, %%d0" : : "r" (val));
	asm("move.l %d0, -(%a7)");
	val = 9988;			
	asm("move.w %0, %%d0" : : "r" (val));
	asm("move.w %d0, -(%a7)");
	val = 16512;			
	asm("move.w %0, %%d0" : : "r" (val));
	asm("move.w %d0, -(%a7)");
	pcb_proc_a->stack -= 8;

	//restore a7
	asm("move.l %0, %%a7" : : "r" (original_a7));
		
	// initialize the process to the correct ready queue
	//put_to_ready(pcb_proc_a);
	pcb_proc_a->state = STATE_NEW;	
	//rtx_dbug_outs((CHAR *)"init_i_proc: exited \r\n");
}