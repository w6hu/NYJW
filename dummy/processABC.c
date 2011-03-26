#include "processABC.h"

void process_a() {

	rtx_dbug_outs((CHAR*)"In process A\r\n");
	register_command(PROCESS_A, 'Z');
	rtx_dbug_outs((CHAR*)"finish registeringA\r\n");
	while (TRUE) {
		int sender_id;
		void* message = receive_message(&sender_id);
		if (sender_id == KCD_ID) {
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

void process_b(){
	while (TRUE){
		int sender_id;
		void * p = receive_message(&sender_id);
		send_message(PROCESS_C, p);
	}
}

//insert from tail, remove from head.
void process_c(){
	int headPtr = 0;
	int tailPtr = 0;
	void * messageQueue[33];
	//initC();
	void * p;
	int sender_id;
	while (TRUE){
		if (headPtr == tailPtr){
			rtx_dbug_outs((CHAR*)"head = tail\r\n");

			p = receive_message(sender_id);
		}else{
			rtx_dbug_outs((CHAR*)"in the else clause\r\n");
			p = messageQueue[headPtr];
			headPtr++;
			headPtr  %= 33;
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
				
				//hibernate
				void* q = request_memory_block();
				*((UINT32 *)q) = COMMAND_WAKEUP10;
				delayed_send(PROCESS_C, q, 10000);
				while (TRUE) {
					rtx_dbug_outs((CHAR*)"sleeping\r\n");
					void* message = receive_message(NULL);
					if (*((UINT32 *)message) == COMMAND_WAKEUP10) {
						release_memory_block(message);
						break;
					} else{
						messageQueue[tailPtr] = message;
						tailPtr++;
						tailPtr %=33;
					}
				}			
			}
			else {
				release_memory_block(p);
			}
		}
		else {
			release_memory_block(p);
		}
		rtx_dbug_outs((CHAR*)"about to release processor\r\n");
		release_processor();
	}
}

void init_process_b (struct PCB* pcb_proc_b, UINT32* stackPtr) {
	pcb_proc_b->next = NULL;
	pcb_proc_b->id = PROCESS_B;
	pcb_proc_b->priority = 1;
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
	val = 4;			
	asm("move.w %0, %%d0" : : "r" (val));
	asm("move.w %d0, -(%a7)");
	val = 16512;			
	asm("move.w %0, %%d0" : : "r" (val));
	asm("move.w %d0, -(%a7)");
	pcb_proc_b->stack -= 8;

	//restore a7
	asm("move.l %0, %%a7" : : "r" (original_a7));
		
	// initialize the process to the correct ready queue
	put_to_ready(pcb_proc_b);
	pcb_proc_b->state = STATE_NEW;	
}


void init_process_c (struct PCB* pcb_proc_c, UINT32* stackPtr) {
	pcb_proc_c->next = NULL;
	pcb_proc_c->id = PROCESS_C;
	pcb_proc_c->priority = 1;
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
	val = 4;			
	asm("move.w %0, %%d0" : : "r" (val));
	asm("move.w %d0, -(%a7)");
	val = 16512;			
	asm("move.w %0, %%d0" : : "r" (val));
	asm("move.w %d0, -(%a7)");
	pcb_proc_c->stack -= 8;

	//restore a7
	asm("move.l %0, %%a7" : : "r" (original_a7));
		
	// initialize the process to the correct ready queue
	put_to_ready(pcb_proc_c);
	pcb_proc_c->state = STATE_NEW;	
}

void init_process_a (struct PCB* pcb_proc_a, UINT32* stackPtr) {
	pcb_proc_a->next = NULL;
	pcb_proc_a->id = PROCESS_A;
	pcb_proc_a->priority = 1;
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
	val = 4;			
	asm("move.w %0, %%d0" : : "r" (val));
	asm("move.w %d0, -(%a7)");
	val = 16512;			
	asm("move.w %0, %%d0" : : "r" (val));
	asm("move.w %d0, -(%a7)");
	pcb_proc_a->stack -= 8;

	//restore a7
	asm("move.l %0, %%a7" : : "r" (original_a7));
		
	// initialize the process to the correct ready queue
	put_to_ready(pcb_proc_a);
	pcb_proc_a->state = STATE_NEW;	
}