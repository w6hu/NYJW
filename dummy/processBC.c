#include "processBC.h"


processB(){

	while (TRUE){
		int sender_id;
		void * p = receive_message(&sender_id);
		send_message(sender_id, PROCESS_C);
	}
}

int queuePtr = 0;
processC(){
	void * messageQueue[32];
	//initC();
	void * p;
	int sender_id;
	while (TRUE){
		if (queuePtr == 0){
			p = receive_message(sender_id);
		}else{
			messageQueue[queuePtr] = q;
			queuePtr--;
		}
		
		if (*((UINT32*)p) == COMMAND_COUNT_REPORT){
			int msg_data = *((UINT32*)p+ 17);
			if (msg_data % 20 == 0){
				*((UINT32* )p +16) = 9;
				*((CHAR* )p +68) = 'P';
				*((CHAR* )p +69) = 'r';
				*((CHAR* )p +70) = 'o';
				*((CHAR* )p +71) = 'c';
				*((CHAR* )p +72) = 'e';
				*((CHAR* )p +73) = 's';
				*((CHAR* )p +74) = 's';
				*((CHAR* )p +75) = ' ';
				*((CHAR* )p +76) = 'C';
				send_message(CRT_ID, p);
				
				void* q = request_memory_block();
				*((UINT32 *)q) = COMMAND_WAKEUP10;
				delayed_send(PROCESS_C, q, 10000);
				while (TRUE) {
					void* message = receive_message(NULL);
					if (*((UINT32 *)message) == COMMAND_WAKEUP10) {
						break;
					} else{
						messageQueue[queuePtr] = p;
						queuePtr++;
					}
				}			
			}
		}
		release_memory_block(p);
		release_processor();
	}
}


void init_procABC (struct PCB* pcb, UINT32* stackPtr)
{	
	//rtx_dbug_outs((CHAR *)"init_i_proc \r\n");
	pcb->next = NULL;
	pcb->id = -3;
	pcb->priority = 0;
	pcb->stack = stackPtr;
	pcb->returning = FALSE;
	pcb->waiting_on = -1;
		
	int val;
	//back up a7
	int original_a7;
	asm("move.l %%a7, %0" : "=r" (original_a7));	
	val = pcb->stack;
	asm("move.l %0, %%a7" : : "r" (val));
	val = pcb;			
	asm("move.l %0, %%d0" : : "r" (val));
	asm("move.l %d0, -(%a7)");
	val = 9988;			
	asm("move.w %0, %%d0" : : "r" (val));
	asm("move.w %d0, -(%a7)");
	val = 16512;			
	asm("move.w %0, %%d0" : : "r" (val));
	asm("move.w %d0, -(%a7)");
	pcb->stack -= 8;

	//restore a7
	asm("move.l %0, %%a7" : : "r" (original_a7));
		
	// initialize the process to the correct ready queue
	//put_to_ready(pcb);
	pcb->state = STATE_NEW;	
	//rtx_dbug_outs((CHAR *)"init_i_proc: exited \r\n");
}
