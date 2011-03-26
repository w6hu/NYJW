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


#include "processBC.h"


process_b(){

	while (TRUE){
		int sender_id;
		void * p = receive_message(&sender_id);
		send_message(PROCESS_C, p);
	}
}

int headPtr = 0;
int tailPtr = 0;
//insert from tail, remove from head.
process_c(){
	void * messageQueue[32];
	//initC();
	void * p;
	int sender_id;
	while (TRUE){
		if (headPtr == tailPtr){
			p = receive_message(sender_id);
		}else{
			p = messageQueue[headPtr];
			headPtr++;
			headPtr  %= 32;
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
						messageQueue[tailPtr] = message;
						tailPtr++;
						tailPtr %=32;
					}
				}			
			}
		}
		release_memory_block(p);
		release_processor();
	}
}


void init_process_b (struct PCB* pcb_proc_b, UINT32* stackPtr) {
	pcb_proc_b->next = NULL;
	pcb_proc_b->id = PROCESS_B;
	pcb_proc_b->priority = 0;
	pcb_proc_b->stack = stackPtr;
	pcb_proc_b->returning = FALSE;
	pcb_proc_b->waiting_on = -1;
		
	int val;
	//back up a7
	int original_a7;
	asm("move.l %%a7, %0" : "=r" (original_a7));	
	val = pcb_proc_b->stack;
	asm("move.l %0, %%a7" : : "r" (val));
	val = process_b;			
	asm("move.l %0, %%d0" : : "r" (val));
	asm("move.l %d0, -(%a7)");
	val = 9988;			
	asm("move.w %0, %%d0" : : "r" (val));
	asm("move.w %d0, -(%a7)");
	val = 16512;			
	asm("move.w %0, %%d0" : : "r" (val));
	asm("move.w %d0, -(%a7)");
	pcb_proc_b->stack -= 8;

	//restore a7
	asm("move.l %0, %%a7" : : "r" (original_a7));
		
	// initialize the process to the correct ready queue
	//put_to_ready(pcb_proc_b);
	pcb_proc_b->state = STATE_NEW;	
	//rtx_dbug_outs((CHAR *)"init_i_proc: exited \r\n");
}


void init_process_c (struct PCB* pcb_proc_c, UINT32* stackPtr) {
	pcb_proc_c->next = NULL;
	pcb_proc_c->id = PROCESS_C;
	pcb_proc_c->priority = 0;
	pcb_proc_c->stack = stackPtr;
	pcb_proc_c->returning = FALSE;
	pcb_proc_c->waiting_on = -1;
		
	int val;
	//back up a7
	int original_a7;
	asm("move.l %%a7, %0" : "=r" (original_a7));	
	val = pcb_proc_c->stack;
	asm("move.l %0, %%a7" : : "r" (val));
	val = process_c;			
	asm("move.l %0, %%d0" : : "r" (val));
	asm("move.l %d0, -(%a7)");
	val = 9988;			
	asm("move.w %0, %%d0" : : "r" (val));
	asm("move.w %d0, -(%a7)");
	val = 16512;			
	asm("move.w %0, %%d0" : : "r" (val));
	asm("move.w %d0, -(%a7)");
	pcb_proc_c->stack -= 8;

	//restore a7
	asm("move.l %0, %%a7" : : "r" (original_a7));
		
	// initialize the process to the correct ready queue
	//put_to_ready(pcb_proc_b);
	pcb_proc_c->state = STATE_NEW;	
	//rtx_dbug_outs((CHAR *)"init_i_proc: exited \r\n");
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