#include "process.h"

// global variable to keep track of the current process that is running
PCB* current_running_process = 0;

// need to import the ready_queue here ~

int release_processor()
{
	int value = 0;
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
	asm("move.l %%a7, %0" : "=r" (val)); //TODO we might not need to save/restore the a7 in the stack, as the PCB keep one copy of its own.
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
	asm("move.w %%sr, %0" : "=r" (val));
	current_running_process->psw = val;
	// set the flag so that it would go to selecting next process
	current_running_process->returning = false;
	// save PC //TODO how exactly do I load teh address ? need some testing to find out
	asm("move.w %%pc, %0" : "=r" (val));
	current_running_process->pc = val;
	
	// if the process is not returning, then we look for another one
	if(current_running_process->returning == false)
	{
		if(current_running_process->state == STATE_READY)
			put_to_ready(current_running_process->id);
		else
			put_to_blocked(current_running_process->id);

		schedule_next_process();
	}
	
	//other wise just return to the calling process and continue
}

// TODO right now it simply choose the highest priority process, we need
// some way to override this behaviour
void schedule_next_process()
{
	// look for the next process.
	// if nothing is selected, the null 
	// process is there at your service.
	for(int i=0; i<5; i++)
	{
		if(ready_queue[i] != null)
		{
			// select the next process
			PCB* to_be_run = ready_queue[i];

			// update ready_queue and PCB data thingy <3
			current_running_process = to_be_run;
			ready_queue[i] = to_be_run->next_process;
			to_be_run->next_process = 0;
			to_be_run->returning = true;

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
<<<<<<< HEAD
			val = *(pop(current_running_process));
			asm("move.l %0, %%a7" : : "r" (val));  //TODO can we just use the PCB stack pointer restore it ?
			val = *(pop(current_running_process));
=======
			val = pop(current_running_process);
			asm("move.l %0, %%a7" : : "r" (val));
			val = pop(current_running_process);
>>>>>>> 4d3102cac66f8661cc2c477d51a772d9693b5cfb
			asm("move.l %0, %%a6" : : "r" (val));
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
			// restore the PWS
			val = current_running_process->psw;
			asm("move.l %0, %%sr" : : "r" (val));
			// restore the PC
			val = current_running_process->pc;
			asm("move.l %0, %%pc" : : "r" (val));
			
			// this is hopefully not run at all ;)
			break;
		}
	}
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
