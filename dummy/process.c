#include "process.h"

// global variable to keep track of the current process that is running
struct PCB* current_running_process = 0;
struct PCB* prev_running_process = 0;

extern struct PCB* ready_queue[5];
extern struct PCB* blocked_queue[1];

void null_process()
{
    while (1) 
    {
        /* execute a rtx primitive to test */
        release_processor_kuma_san();
    }
}

void init_null_process( struct PCB* pcb_null_process, UINT32* process_start)
{	
	pcb_null_process->next = NULL;
	pcb_null_process->id = -2;
	//pcb_null_process->state = STATE_READY;
	pcb_null_process->priority = 4;
	pcb_null_process->psw = 9984;   // assuming 9984 is the nomal initial state ... eh ?
	pcb_null_process->pc = null_process; //point pc to entry point of code
	pcb_null_process->stack = process_start; // where exactly is the process stack ?
	pcb_null_process->returning = FALSE;
	pcb_null_process->waiting_on = -1;
		
	int val;
	//back up a7
	int original_a7;
	asm("move.l %%a7, %0" : "=r" (original_a7));	
	val = pcb_null_process->stack;
	asm("move.l %0, %%a7" : : "r" (val));
	val = pcb_null_process->pc;			
	asm("move.l %0, %%d0" : : "r" (val));
	asm("move.l %d0, -(%a7)");
	asm("move.l %d0, -(%a7)");
	asm("move.l %d0, -(%a7)");
	asm("move.l %d0, -(%a7)");
	pcb_null_process->stack -= 4;

	//restore a7
	asm("move.l %0, %%a7" : : "r" (original_a7));
		
	// initialize the process to the correct ready queue
	put_to_ready(pcb_null_process);
	pcb_null_process->state = STATE_NEW;	
}

int release_processor_kuma_san()
{	
	// set state and put the process to ready
	if(current_running_process->state == STATE_RUNNING)
		put_to_ready (current_running_process);
	
	schedule_next_process();
	
	//other wise just return to the calling process and continue
	return RTX_SUCCESS;
}

void schedule_next_process()
{
	// look for the next process.
	// if nothing is selected, the null 
	// process is there at your service.
	int i=0;
	for(i; i<5; i++)
	{
		if(ready_queue[i] != NULL)
		{			
			prev_running_process = current_running_process;
			
			// select the next process
			current_running_process = ready_queue[i];					
			ready_queue[i] = current_running_process->next;
			current_running_process->next = NULL;
			
			asm( "TRAP #0" );			

			break;
		}
	}
}

VOID stack_pointer_switcher( VOID )
{
	asm("move.l %a6, -(%a7)");
	
	int val;
	if(prev_running_process != NULL)
	{
		asm("move.l %%a7, %0" : "=r" (val));
		prev_running_process->stack = val;	
	}
	
	val = current_running_process->stack;
	asm("move.l %0, %%a7" :  : "r" (val));	

	if(current_running_process->state == STATE_NEW)
	{
		current_running_process->state = STATE_RUNNING;
		asm("rts");
	}
	
	current_running_process->state = STATE_RUNNING;
	asm("move.l (%a7)+, %a6");	
}

struct PCB* get_process_from_ID(int process_id)
{
	int i = 0;
	extern struct PCB p [6];
	for (i; i < 6; i++) {
		if (p[i].id == process_id) {
			return &(p[i]);
		}
	}
	return NULL;
}

int get_process_number_from_ID(int process_id)
{
	int i = 0;
	extern struct PCB p [6];
	for (i; i < 6; i++) {
		if (p[i].id == process_id) {
			return i;
		}
	}
	return -1;
}

int get_process_ID() 
{
	return current_running_process->id;
}

int get_process_priority_usagi_san(int process_id)
{
	struct PCB* to_be_accessed = get_process_from_ID(process_id);
	if(to_be_accessed != NULL)
	{
		return to_be_accessed->priority;
	}
}

int process_exists(int process_id)
{
	int i = 0;
	extern struct PCB p [6];
	for (i; i < 6; i++) {
		if (p[i].id == process_id) {
			return TRUE;
		}
	}
	return FALSE;
}

int is_waiting_for(int process_ID, int sender_ID)
{
	// sanity check
	if(process_exists(process_ID))
	{
		return (get_process_from_ID(process_ID)->waiting_on == sender_ID);
	}
	
	return FALSE;
}

void set_process_state(int process_id, int process_state)
{
	struct PCB* to_be_modified = get_process_from_ID(process_id);
	if(to_be_modified != NULL)
	{
		to_be_modified->state = process_state;
	}
}

