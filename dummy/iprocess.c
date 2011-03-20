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
	BYTE temp;
	temp = SERIAL1_USR;
	BOOLEAN caught = FALSE;
	//if data is waiting
	if (temp & 1 && caught == FALSE){
		charIn = SERIAL1_RD;
				
		struct PCB* backup = current_running_process;
		current_running_process = &keyboard_i_proc; 
		
		void * msg = request_memory_block();
		*((CHAR *)msg + 100) =  charIn;
		*((UINT32*)msg) = 42;//filler value
		//change to the proper state later.
		
		send_message_jessie(-5, msg);//send to the CRT first.
		// set automic here by disabling the interrupt
		
		void * msg2 = request_memory_block();
		*((CHAR *)msg2 + 100) =  charIn;
		//*((UINT32*)msg2) = COMMAND_KEYBOARD;
		*((UINT32*)msg2) = 42;//filler value
		send_message_jessie(-4, msg2);//send to the KCD next.
		current_running_process = backup;
		caught = TRUE;
	}else if (temp & 4)
	// if port is ready to accept data
	{
		struct PCB* backup = current_running_process;
		current_running_process = &keyboard_i_proc; 
		caught = FALSE;
		int sender_id;
		void * block = receive_message_jessie(&sender_id, 0);
		charOut = *((CHAR*)block +100);
		//rtx_dbug_out_char (charOut);
		//rtx_dbug_outs ((CHAR*) "\r\n");
		//charOut = charIn;
		SERIAL1_WD = charOut;
		if (block!= NULL) {
			release_memory_block(block);
		}
		//rtx_dbug_outs((CHAR*)"you typed ");
		//rtx_dbug_out_char(charOut);
		//rtx_dbug_outs((CHAR*)"\n\r");
		SERIAL1_IMR = 2;//disable interrupt
		if (charOut == CR){
			while (! temp & 4){
				temp = SERIAL1_USR;
			}// blocking here?
			charOut = LF;
			SERIAL1_WD = charOut;	
		}
		current_running_process = backup;
		//rtx_dbug_outs((CHAR*)"done putting stuff to the other screen\n\r");
	}
}

void init_keyboard_i_proc (struct PCB* pcb_keyboard_i_proc, UINT32* stackPtr)
{	
	rtx_dbug_outs((CHAR *)"init_i_proc \r\n");
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
	rtx_dbug_outs((CHAR *)"init_i_proc: exited \r\n");
}

void init_timer_i_proc (struct PCB* pcb_keyboard_i_proc, UINT32* stackPtr)
{	
	rtx_dbug_outs((CHAR *)"init_i_proc \r\n");
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
	rtx_dbug_outs((CHAR *)"init_i_proc: exited \r\n");
}
