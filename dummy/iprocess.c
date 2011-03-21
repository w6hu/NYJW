/*
5.2.2 UART I-Process
The UART i-process uses interrupts for both the transmission and receiving of characters from the serial port.
 No polling or busy waiting strategies may be implemented. 
 The UART i-process forwards characters (or commands) received to the KCD, 
 and also responds to messages received from the CRT display process to transmit characters to the serial port.
*/

#include "iprocess.h"
/*
*/

CHAR charOut;
CHAR charIn;

extern struct PCB* current_running_process;
extern struct PCB keyboard_i_proc;

void uart_i_process(){
	asm( "move.w #0x2700,%sr" );
	volatile BYTE temp;
	temp = SERIAL1_USR;
	SERIAL1_IMR = 2;//disable interrupt
	//if data is waiting
	if (temp & 1){
		charIn = SERIAL1_RD;
		if ((int)charIn ==	18){//ctrl+r
			print_ready_queue();
		}else if ((int )charIn == 2){//ctrl+b
			print_blocked_queues();
		}else{
			struct PCB* backup = current_running_process;
			current_running_process = &keyboard_i_proc; 
			
			void * msg = s_request_memory_block_yishi(0);
			*((UINT32 *)msg) = COMMAND_KEYBOARD;
			*((UINT32 *)msg + 16) =  1;
			
			*((CHAR *)msg + 68) =  charIn;

			send_message_jessie(-5, msg);//send to the CRT first.
			// set automic here by disabling the interrupt
				
			void * msg2 = s_request_memory_block_yishi(0);
			*((UINT32 *)msg2) = COMMAND_KEYBOARD;
			*((UINT32 *)msg2 + 16) =  1;
			*((CHAR *)msg2 + 68) =  charIn;
			
			//rtx_dbug_outs((CHAR *)"IPROCESS : sending message to KCD\r\n");
			send_message_jessie(-4, msg2);//send to the KCD next.
			current_running_process = backup;
			}
	}else if (temp & 4)
	// if port is ready to accept data
	{

		
		SERIAL1_IMR = 2;
		struct PCB* backup = current_running_process;
		current_running_process = &keyboard_i_proc; 
		int sender_id;
		void * block;
		block = receive_message_jessie(&sender_id, 0);
		if (sender_id == -5){
			temp = SERIAL1_USR;
			charOut = *((CHAR*)block +68);
			while (! (temp&4)){
				temp = SERIAL1_USR;								
			}
			charOut = *((CHAR*)block +68);
			SERIAL1_WD = charOut;
			if (charOut == CR){
				temp = SERIAL1_USR;
				while (! (temp & 4)){
					temp = SERIAL1_USR;
				}// blocking here?
				charOut = LF;
				SERIAL1_WD = charOut;	
			}
		}
		if (block!= NULL) {
			release_memory_block(block);
		}
		current_running_process = backup;
	}
}

void init_keyboard_i_proc (struct PCB* pcb_keyboard_i_proc, UINT32* stackPtr)
{	
	//rtx_dbug_outs((CHAR *)"init_i_proc \r\n");
	pcb_keyboard_i_proc->next = NULL;
	pcb_keyboard_i_proc->id = -3;
	pcb_keyboard_i_proc->priority = 0;
	pcb_keyboard_i_proc->stack = stackPtr;
	pcb_keyboard_i_proc->returning = FALSE;
	pcb_keyboard_i_proc->waiting_on = -1;
		
	int val;
	//back up a7
	int original_a7;
	asm("move.l %%a7, %0" : "=r" (original_a7));	
	val = pcb_keyboard_i_proc->stack;
	asm("move.l %0, %%a7" : : "r" (val));
	val = pcb_keyboard_i_proc;			
	asm("move.l %0, %%d0" : : "r" (val));
	asm("move.l %d0, -(%a7)");
	val = 9988;			
	asm("move.w %0, %%d0" : : "r" (val));
	asm("move.w %d0, -(%a7)");
	val = 16512;			
	asm("move.w %0, %%d0" : : "r" (val));
	asm("move.w %d0, -(%a7)");
	pcb_keyboard_i_proc->stack -= 8;

	//restore a7
	asm("move.l %0, %%a7" : : "r" (original_a7));
		
	// initialize the process to the correct ready queue
	//put_to_ready(pcb_keyboard_i_proc);
	pcb_keyboard_i_proc->state = STATE_NEW;	
	//rtx_dbug_outs((CHAR *)"init_i_proc: exited \r\n");
}

void init_timer_i_proc (struct PCB* pcb_keyboard_i_proc, UINT32* stackPtr)
{	
	//rtx_dbug_outs((CHAR *)"init_i_proc \r\n");
	pcb_keyboard_i_proc->next = NULL;
	pcb_keyboard_i_proc->id = TIMER_INTERRUPT;
	pcb_keyboard_i_proc->priority = 0;
	pcb_keyboard_i_proc->stack = stackPtr;
	pcb_keyboard_i_proc->returning = FALSE;
	pcb_keyboard_i_proc->waiting_on = -1;
		
	int val;
	//back up a7
	int original_a7;
	asm("move.l %%a7, %0" : "=r" (original_a7));	
	val = pcb_keyboard_i_proc->stack;
	asm("move.l %0, %%a7" : : "r" (val));
	val = handle_timer;			
	asm("move.l %0, %%d0" : : "r" (val));
	asm("move.l %d0, -(%a7)");
	val = 8196;			
	asm("move.w %0, %%d0" : : "r" (val));
	asm("move.w %d0, -(%a7)");
	val = 16512;			
	asm("move.w %0, %%d0" : : "r" (val));
	asm("move.w %d0, -(%a7)");
	pcb_keyboard_i_proc->stack -= 8;

	//restore a7
	asm("move.l %0, %%a7" : : "r" (original_a7));
		
	// initialize the process to the correct ready queue
	//put_to_ready(pcb_keyboard_i_proc);
	pcb_keyboard_i_proc->state = STATE_NEW;	
	//rtx_dbug_outs((CHAR *)"init_i_proc: exited \r\n");
}
