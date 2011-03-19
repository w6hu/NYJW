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
	//rtx_dbug_outs((CHAR*)"key interrupt detected!\n\r");
	BYTE temp;
	temp = SERIAL1_USR;
	BOOLEAN caught;
	//if data is waiting
	if (temp & 1){
		charIn = SERIAL1_RD;
		SERIAL1_IMR = 3;//enable interrupt
		void * msg = request_memory_block();
		
		*((CHAR *)msg + 100) =  charIn;
		//*((UINT32*)msg) = COMMAND_KEYBOARD;
		*((UINT32*)msg) = 42;//filler value
		rtx_dbug_outs((CHAR*)"creating memory location @");
		rtx_dbug_out_num((int)msg);
		struct PCB* backup = current_running_process;
		*(current_running_process) = keyboard_i_proc; 
		send_message(-5, msg);//send to the CRT first.
		send_message(-4, msg);//send to the KCD next.
		current_running_process = backup;
		caught = TRUE;
	}else if (temp & 4)
	// if port is ready to accept data
	{
		//rtx_dbug_outs((CHAR*)"putting char to the other scren\n\r");
		caught = FALSE;
		void * block = receive_message(-5);
		charOut = *((CHAR*)block +100);
		SERIAL1_WD = charOut;
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
		//rtx_dbug_outs((CHAR*)"done putting stuff to the other screen\n\r");
	}
}

void init_keyboard_i_proc (struct PCB* pcb_keyboard_i_proc, UINT32* stackPtr)
{	
	rtx_dbug_outs((CHAR *)"init_i_proc \r\n");
	pcb_keyboard_i_proc->next = NULL;
	pcb_keyboard_i_proc->id = -3;
	pcb_keyboard_i_proc->priority = 3;
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
	val = 4;			
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
