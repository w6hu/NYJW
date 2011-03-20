#include "crt.h"

extern struct PCB* current_running_process;
extern struct PCB p [NUM_PROCESS];
//pid =-5!!
void crt(){
	while (TRUE){
		SERIAL1_IMR = 2;
		int sender_id;
		struct PCB* backup;
//		backup = current_running_process;
//		current_running_process = &p[8];
		
		//rtx_dbug_outs((CHAR *)"crt: before message:");
		
		void* block = receive_message(&sender_id);//receive from -3
		//rtx_dbug_outs("sender = ");
		//rtx_dbug_out_num(sender_id);
		if (sender_id == -3 || sender_id == -6){//if sender is i-process or timer
			send_message(-3,block);
//			current_running_process = backup;
			SERIAL1_IMR = 3;
		}
	}
}


void init_crt (struct PCB* pcb_crt, UINT32* stackPtr){

	rtx_dbug_outs((CHAR *)"init_crt \r\n");
	pcb_crt->next = NULL;
	pcb_crt->id = -5;
	pcb_crt->priority = 0;//you must be very important!
	pcb_crt->stack = stackPtr;
	pcb_crt->returning = FALSE;
	pcb_crt->waiting_on = -1;
		
	int val;
	//back up a7
	int original_a7;
	asm("move.l %%a7, %0" : "=r" (original_a7));	
	val = pcb_crt->stack;
	asm("move.l %0, %%a7" : : "r" (val));
	val = crt;			
	asm("move.l %0, %%d0" : : "r" (val));
	asm("move.l %d0, -(%a7)");
	val = 4;		// TODO change this to a kernel level, but at the same time, leave interrupt enabled	
	asm("move.w %0, %%d0" : : "r" (val));
	asm("move.w %d0, -(%a7)");
	val = 16512;			
	asm("move.w %0, %%d0" : : "r" (val));
	asm("move.w %d0, -(%a7)");
	pcb_crt->stack -= 8;

	//restore a7
	asm("move.l %0, %%a7" : : "r" (original_a7));
		
	// initialize the process to the correct ready queue
	put_to_ready(pcb_crt);
	pcb_crt->state = STATE_NEW;	
	rtx_dbug_outs((CHAR *)"init_crt: exited \r\n");
}