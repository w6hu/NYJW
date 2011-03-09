#include "process.h"

// global variable to keep track of the current process that is running
struct PCB* current_running_process = 0;
struct PCB* prev_running_process = 0;
struct PCB* imma_epeen_process = 0;

extern struct PCB* ready_queue[5];
extern struct PCB* blocked_queue[6];

void null_process()
{
    while (1) 
    {
	    rtx_dbug_outs((CHAR *)"Hey APLLE! Hey Apple! Hey Apple !\r\n");
        /* execute a rtx primitive to test */
        release_processor_kuma_san();
    }
}

void init_null_process( struct PCB* pcb_null_process, UINT32* process_start)
{	
	rtx_dbug_outs((CHAR *)"rtx: Initializing the null process\r\n");
	pcb_null_process->next = NULL;
	pcb_null_process->id = -2;
	pcb_null_process->state = STATE_READY;
	pcb_null_process->priority = 0;
	pcb_null_process->psw = 9984;   // assuming 9984 is the nomal initial state ... eh ?
	pcb_null_process->pc = null_process; //point pc to entry point of code
	pcb_null_process->stack = process_start; // where exactly is the process stack ?
	pcb_null_process->returning = FALSE;
	pcb_null_process->waiting_on = -1;
		
	int j = 0;
	for (j; j < 14; j++) {
		push(pcb_null_process, 0);
	}
		
	// initialize the process to the correct ready queue
	put_to_ready(pcb_null_process);
}

int release_processor_kuma_san()
{
	rtx_dbug_outs((CHAR *)"rtx: Kuma san Ohaiyou !!\r\n");
	
	// set state and put the process to ready
	// TODO does this make sense ?
	if(current_running_process->state == STATE_RUNNING)
		put_to_ready (current_running_process);
	
	schedule_next_process();
	
	//other wise just return to the calling process and continue
	return RTX_SUCCESS;
}

// TODO right now it simply choose the highest priority process, we need
// some way to override this behaviour
void schedule_next_process()
{
	rtx_dbug_outs((CHAR *)"rtx: In scheduler\r\n");

	// look for the next process.
	// if nothing is selected, the null 
	// process is there at your service.
	int i=0;
	rtx_dbug_outs((CHAR *)"rtx: before the scheduling loop\r\n");
	for(i; i<5; i++)
	{
		if(ready_queue[i] != NULL)
		{
			rtx_dbug_outs((CHAR *)"rtx: Found a ready process, begin to switch .... !\r\n");
			
			prev_running_process = current_running_process;
			
			// select the next process
			current_running_process = ready_queue[i];					
			ready_queue[i] = current_running_process->next;
			current_running_process->next = NULL;
			
			asm( "TRAP #0" );			

			break;
		}
	}
	rtx_dbug_outs((CHAR *)"rtx: after the scheduling loop the loop\r\n");
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
	if(process_exists(process_ID) && process_exists(sender_ID))
	{
		if(current_running_process->id == sender_ID)
		{
			return (get_process_from_ID(process_ID)->waiting_on == sender_ID);
		}
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

void set_process_to_run(int process_id)
{
	struct PCB* to_be_run = get_process_from_ID(process_id);
	if(to_be_run != NULL)
	{
		imma_epeen_process = to_be_run;
	}
}
