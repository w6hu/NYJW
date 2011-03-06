#include "process.h"

// global variable to keep track of the current process that is running
struct PCB* current_running_process = 0;

extern struct PCB* ready_queue[5];
extern struct PCB* blocked_queue[6];

// need to import the ready_queue here ~

int release_processor_kuma_san()
{
	rtx_dbug_outs((CHAR *)"rtx: Kuma san Ohaiyou !!\r\n");
	int val = 0;
	// save regiser a0 ~ a7, d0 ~ d7 // should I backup a7??
	asm("move.l %%a0, %0" : "=r" (val));
	push(current_running_process, val);
	asm("move.l %%a1, %0" : "=r" (val));
	push(current_running_process, val);
	asm("move.l %%a2, %0" : "=r" (val));
	push(current_running_process, val);
	asm("move.l %%a3, %0" : "=r" (val));
	push(current_running_process, val);
	asm("move.l %%a4, %0" : "=r" (val));
	push(current_running_process, val);
	asm("move.l %%a5, %0" : "=r" (val));
	push(current_running_process, val);
	asm("move.l %%a6, %0" : "=r" (val));
	push(current_running_process, val);
	asm("move.l %%d0, %0" : "=r" (val));
	push(current_running_process, val);
	asm("move.l %%d1, %0" : "=r" (val));
	push(current_running_process, val);
	asm("move.l %%d2, %0" : "=r" (val));
	push(current_running_process, val);
	asm("move.l %%d3, %0" : "=r" (val));
	push(current_running_process, val);
	asm("move.l %%d4, %0" : "=r" (val));
	push(current_running_process, val);
	asm("move.l %%d5, %0" : "=r" (val));
	push(current_running_process, val);
	asm("move.l %%d6, %0" : "=r" (val));
	push(current_running_process, val);
	asm("move.l %%d7, %0" : "=r" (val));
	push(current_running_process, val);
	
	// save PSW
	asm("move.w %sr, %d0");
	asm("move.w %%d0, %0" : "=r" (val));
	current_running_process->psw = val;
	
	// set the flag so that it would go to selecting next process
	current_running_process->returning = FALSE;
	
	// save PC //TODO how exactly do I load the address ? need some testing to find out
	asm("lea 0(%pc), %a0");
	asm("move.l %%a0, %0" : "=r" (val));
	current_running_process->pc = val;
	
	// if the process is not returning, then we look for another one
	if(current_running_process->returning == FALSE)
	{
		if(current_running_process->state == STATE_READY)
			put_to_ready(current_running_process);
		else
			put_to_blocked(current_running_process->waiting_on, current_running_process);
	
		schedule_next_process();
	}
	
	//other wise just return to the calling process and continue
	return RTX_SUCCESS;
}

// TODO right now it simply choose the highest priority process, we need
// some way to override this behaviour
void schedule_next_process()
{
	// look for the next process.
	// if nothing is selected, the null 
	// process is there at your service.
	int i=0;
	rtx_dbug_outs((CHAR *)"rtx: before the scheduling loop\r\n");
	for(i; i<5; i++)
	{
	rtx_dbug_outs((CHAR *)"looping in scheler\r\n");
		if(ready_queue[i] != NULL)
		{
			// select the next process
			struct PCB* to_be_run = ready_queue[i];

			// update ready_queue and PCB data thingy <3
			current_running_process = to_be_run;
			ready_queue[i] = to_be_run->next;
			to_be_run->next = 0;
			to_be_run->returning = FALSE;
	
			// restore the register d7 ~ d0, a7 ~ a0
			int val = 0;
			val = pop(current_running_process);
			asm("move.l %0, %%d7" : : "r" (val));
			val = pop(current_running_process);
			asm("move.l %0, %%d6" : : "r" (val));
			val = pop(current_running_process);
			asm("move.l %0, %%d5" : : "r" (val));
			val = pop(current_running_process);
			asm("move.l %0, %%d4" : : "r" (val));
			val = pop(current_running_process);
			asm("move.l %0, %%d3" : : "r" (val));
			val = pop(current_running_process);
			asm("move.l %0, %%d2" : : "r" (val));
			val = pop(current_running_process);
			asm("move.l %0, %%d1" : : "r" (val));
			val = pop(current_running_process);
			asm("move.l %0, %%d0" : : "r" (val));
			val = pop(current_running_process);		  // TODO a6 is some magical register that, if modified, crash the OS ....
			if(val != 0)							  // a hack.  Since the frame pointer can never be 0, we initialize it 	
				asm("move.l %0, %%a6" : : "r" (val)); // to 0, and if it is a 0, don't bother pushing it onto a6 ^^; 
			val = pop(current_running_process);
			asm("move.l %0, %%a5" : : "r" (val));
			val = pop(current_running_process);
			asm("move.l %0, %%a4" : : "r" (val));
			val = pop(current_running_process);
			asm("move.l %0, %%a3" : : "r" (val));
			val = pop(current_running_process);
			asm("move.l %0, %%a2" : : "r" (val));
			val = pop(current_running_process);
			asm("move.l %0, %%a1" : : "r" (val));
			val = pop(current_running_process);
			asm("move.l %0, %%a0" : : "r" (val));
			
			rtx_dbug_outs((CHAR *)"rtx: How can you NOT do this to me ~ I am lonely <3 1!\r\n");
			// restore the PWS
			val = current_running_process->psw;
			asm("move.l %0, %%d0" : : "r" (val));
			asm("move.w %d0, %sr");
			rtx_dbug_outs((CHAR *)"rtx: How can you NOT do this to me ~ I am lonely <3 2!\r\n");
			// restore the PC
			val = current_running_process->pc;
			asm("move.l %0, %%a1" : : "r" (val));
			rtx_dbug_outs((CHAR *)"rtx: How can you NOT do this to me ~ I am lonely <3 13!\r\n");
			asm("jmp (%a1)");
		
			// this is hopefully not run at all ;
			rtx_dbug_outs((CHAR *)"rtx: SOMETHING IS WRONG, SHOULD NOT REACH HERE!\r\n");
			while(1);
			break;
		}
	}
	rtx_dbug_outs((CHAR *)"rtx: after the scheduling loop the loop\r\n");
}

//Copied to init.c
/*
void put_to_ready(int process_id)
{
	// get the process from process_id
	int priority = -1;
	PCB* temp = NULL;
	PCB* one_pcb = getPCB(process_id);

	// if the process is in the block queue, remove it from the block queue
	remove_from_block(process_id);

	// get the priority of the process, 
	// hopefully it is between 0 and 4, otherwise we got array out of bound
	priority = one_pcb->priority;
	// get the corresponding ready queue from the pritority
	temp = ready_queue[priority];
	
	if(temp != NULL && temp->ID == process_id)
		return;

	while(temp != NULL && temp->next_process != NULL)
	{

		// if the process is alreay in there, don't bother doing anything (sanity check)
		if(temp->ID == process_id)
			return;

		temp = temp->next_process;
	}
	
	// at this point, temp should be the tail of our lovely queue.
	// add the process into the corresponding queue and update the PCB
	temp->next_process = one_pcb;
	one_pcb->next_process = NULL;
	one_pcb->state = STATE_READY;
}
*/
