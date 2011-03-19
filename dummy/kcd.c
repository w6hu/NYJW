#include "kcd.h"

void kcd()
{
    while (1) 
    {
		rtx_dbug_outs((CHAR *)"kcd: calling recieve message \r\n");
        receive_message (NULL);
		
		rtx_dbug_outs((CHAR *)"kcd: recieved message \r\n");
    }
}

void init_kcd (struct PCB* pcb_kcd, UINT32* stackPtr)
{	
	rtx_dbug_outs((CHAR *)"init_kcd \r\n");
	pcb_kcd->next = NULL;
	pcb_kcd->id = -4;
	pcb_kcd->priority = 2;
	pcb_kcd->stack = stackPtr;
	pcb_kcd->returning = FALSE;
	pcb_kcd->waiting_on = -1;
		
	int val;
	//back up a7
	int original_a7;
	asm("move.l %%a7, %0" : "=r" (original_a7));	
	val = pcb_kcd->stack;
	asm("move.l %0, %%a7" : : "r" (val));
	val = kcd;			
	asm("move.l %0, %%d0" : : "r" (val));
	asm("move.l %d0, -(%a7)");
	val = 4;			
	asm("move.w %0, %%d0" : : "r" (val));
	asm("move.w %d0, -(%a7)");
	val = 16512;			
	asm("move.w %0, %%d0" : : "r" (val));
	asm("move.w %d0, -(%a7)");
	pcb_kcd->stack -= 8;

	//restore a7
	asm("move.l %0, %%a7" : : "r" (original_a7));
		
	// initialize the process to the correct ready queue
	put_to_ready(pcb_kcd);
	pcb_kcd->state = STATE_NEW;	
	rtx_dbug_outs((CHAR *)"init_kcd: exited \r\n");
}