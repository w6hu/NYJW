#include "crt.h"

extern struct PCB* current_running_process;
extern struct PCB p [NUM_PROCESS];

CHAR  escapeHack[] = "\x1b[01;68H";
CHAR restoreHack[] = "\x1b[10;10H";
int line = 1;
int column = 1;

void crt(){
	while (TRUE){
		SERIAL1_IMR = 2;
		int sender_id;
		struct PCB* backup;
		
		void* block = receive_message(&sender_id);//receive from -3
		if ((sender_id == KEYBOARD_INTERRUPT || sender_id == WALL_CLOCK_ID || sender_id == SET_PRIORITY_ID || sender_id == PROCESS_C) && block != NULL){//if sender is i-process or timer
			UINT32 type = *((UINT32 *)block);
			//rtx_dbug_outs((CHAR*)"ERROR type");
			//rtx_dbug_out_num(type);
			//rtx_dbug_outs((CHAR*)"\r\n");
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
			//rtx_dbug_outs((CHAR*) "length =");
			//rtx_dbug_out_num(length);
			//rtx_dbug_outs((CHAR*) "\n");
			int j = 0;
			CHAR charOut;
			CHAR temp = SERIAL1_USR;
			if (sender_id == WALL_CLOCK_ID){
				int j = 0;
				for (j; j < 8 ; j++){
					void * newBlock  = s_request_memory_block_yishi(0);
					*((int* )newBlock+16) = 1;
					*((CHAR* )newBlock+68) = escapeHack[j];
					send_message(KEYBOARD_INTERRUPT,newBlock);
					temp = SERIAL1_USR;
					while (! (temp&4)){
						temp = SERIAL1_USR;
						//rtx_dbug_outs((CHAR*)"output not ready yet!!\n\r");					
					}
					SERIAL1_IMR = 3;
				}
			}
			
			for (j = 0; j < length; j++){

				charOut = *((CHAR*)block +68+j);
				void * newBlock  = s_request_memory_block_yishi(0);
				*((int* )newBlock+16) = 1;
				*((CHAR* )newBlock+68) = charOut;
				send_message(KEYBOARD_INTERRUPT,newBlock);
				temp = SERIAL1_USR;
				while (! (temp&4)){
					temp = SERIAL1_USR;
					//rtx_dbug_outs((CHAR*)"output not ready yet!!\n\r");					
				}
				SERIAL1_IMR = 3;
				if (sender_id != WALL_CLOCK_ID){
					column++;
					if (column >=80)
						line++;
					column = column % 76;
					if (charOut == CR){
						line ++;
						column = 1;
					}
					if (charOut == BKSP)
						column = column -2;
						column = column %76;
				}
								
			}
			

			if (sender_id == WALL_CLOCK_ID){
				restoreHack[2] = line /10 +48;
				restoreHack[3] = line %10 +48;
				restoreHack[5] = column/10+ 48;
				restoreHack[6] = column%10 +48;
				
				int j = 0;
				for (j;j< 8;j++){
					void * newBlock  = s_request_memory_block_yishi(0);
					*((int* )newBlock+16) = 1;
					*((CHAR* )newBlock+68) = restoreHack[j];
					send_message(KEYBOARD_INTERRUPT,newBlock);
					temp = SERIAL1_USR;
					while (! (temp&4)){
						temp = SERIAL1_USR;
						//rtx_dbug_outs((CHAR*)"output not ready yet!!\n\r");					
					}
					SERIAL1_IMR = 3;
				}
			}
			
			
		}
		if (block!= NULL){
			release_memory_block(block);
		}
	}
}


void init_crt (struct PCB* pcb_crt, UINT32* stackPtr){

	//rtx_dbug_outs((CHAR *)"init_crt \r\n");
	pcb_crt->next = NULL;
	pcb_crt->id = CRT_ID;
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
	//rtx_dbug_outs((CHAR *)"init_crt: exited \r\n");
}