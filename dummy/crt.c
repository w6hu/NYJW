#include "crt.h"

extern struct PCB* current_running_process;
extern struct PCB p [NUM_PROCESS];
//pid =-5!!
void crt(){
	while (TRUE){
		SERIAL1_IMR = 2;
		int sender_id;
		struct PCB* backup;
		
		void* block = receive_message(&sender_id);//receive from -3
		if ((sender_id == -3 || sender_id == -6 || sender_id == -7) && block != NULL){//if sender is i-process or timer
			UINT32 type = *((UINT32 *)block);
			rtx_dbug_outs((CHAR*)"ERROR type");
			rtx_dbug_out_num(type);
			rtx_dbug_outs((CHAR*)"\r\n");
			if (type == COMMAND_ERROR){//remove the false to enable error printing.
				//release_memory_block(block);
				//block = request_memory_block();
				*((UINT32 *)block +16) = 17;
				*((CHAR *)block + 68) = 'I';
				*((CHAR *)block + 69) = 'n';
				*((CHAR *)block + 70) = 'v';
				*((CHAR *)block + 71) = 'a';
				*((CHAR *)block + 72) = 'l';
				*((CHAR *)block + 73) = 'i';
				*((CHAR *)block + 74) = 'd';
				*((CHAR *)block + 75) = ' ';
				*((CHAR *)block + 76) = 'C';
				*((CHAR *)block + 77) = 'o';
				*((CHAR *)block + 78) = 'm';
				*((CHAR *)block + 79) = 'm';
				*((CHAR *)block + 80) = 'a';
				*((CHAR *)block + 81) = 'n';
				*((CHAR *)block + 82) = 'd';
				*((CHAR *)block + 83) = '\r';
				*((CHAR *)block + 84) = '\n';
				
			}
			UINT32 length = *((UINT32*)block+16);
			int j = 0;
			CHAR charOut;
			for (j; j < length; j++){
				CHAR temp = SERIAL1_USR;
				
				charOut = *((CHAR*)block +68+j);
				void * newBlock  = request_memory_block();
				*((int* )newBlock+16) = 1;
				*((CHAR* )newBlock+68) = charOut;
				send_message(-3,newBlock);
				while (! (temp&4)){
					temp = SERIAL1_USR;
					rtx_dbug_outs((CHAR*)"output not ready yet!!\n\r");					
				}
				SERIAL1_IMR = 3;				
			}
		}
		if (block!= NULL){
			release_memory_block(block);
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