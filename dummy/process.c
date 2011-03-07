#include "process.h"

// global variable to keep track of the current process that is running
struct PCB* current_running_process = 0;
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
		
	//back up a7
	int original_a7;
	asm("move.l %%a7, %0" : "=r" (original_a7));
		
	int val = pcb_null_process->stack;
	asm("move.l %0, %%a7" : : "r" (val));
	val = pcb_null_process->pc;
	
	asm("move.l %0, %%d0" : : "r" (val));
	asm("move.l %d0, -(%a7)");
	asm("move.l %d0, -(%a7)");

	
	//restore a7
	asm("move.l %0, %%a7" : : "r" (original_a7));
					
	// initialize the process to the correct ready queue
	put_to_ready(pcb_null_process);
}

VOID c_trap_handler( VOID )
{
    rtx_dbug_outs( (CHAR *) "Trap Handler!!\n\r" );
}

int release_processor_kuma_san()
{
	rtx_dbug_outs((CHAR *)"rtx: Kuma san Ohaiyou !!\r\n");
	
	asm("move.l %a0, -(%a7)");
	asm("move.l %a1, -(%a7)");
	asm("move.l %a2, -(%a7)");
	asm("move.l %a3, -(%a7)");
	asm("move.l %a4, -(%a7)");
	asm("move.l %a5, -(%a7)");
	asm("move.l %a6, -(%a7)");
	asm("move.l %d0, -(%a7)");
	asm("move.l %d1, -(%a7)");
	asm("move.l %d2, -(%a7)");
	asm("move.l %d3, -(%a7)");
	asm("move.l %d4, -(%a7)");
	asm("move.l %d5, -(%a7)");
	asm("move.l %d6, -(%a7)");
	asm("move.l %d7, -(%a7)");

	// set state and put the process to ready
	current_running_process->state = STATE_READY;
	put_to_ready (current_running_process);
	
	schedule_next_process();
	
	asm("move.l (%a7)+, %d7");
	asm("move.l (%a7)+, %d6");
	asm("move.l (%a7)+, %d5");
	asm("move.l (%a7)+, %d4");
	asm("move.l (%a7)+, %d3");
	asm("move.l (%a7)+, %d2");
	
	asm("move.l (%a7)+, %d1");
	asm("move.l (%a7)+, %d0");
	asm("move.l (%a7)+, %a6");
	asm("move.l (%a7)+, %a5");
	asm("move.l (%a7)+, %a4");
	asm("move.l (%a7)+, %a3");
	asm("move.l (%a7)+, %a2");
	asm("move.l (%a7)+, %a1");
	asm("move.l (%a7)+, %a0");
	
	//other wise just return to the calling process and continue
	return RTX_SUCCESS;
}

// TODO right now it simply choose the highest priority process, we need
// some way to override this behaviour
void schedule_next_process()
{
	rtx_dbug_outs((CHAR *)"rtx: In scheduler\r\n");
	int val;
	if(current_running_process != NULL)
	{
		rtx_dbug_outs((CHAR *)"rtx: Kuma san has revived !\r\n");
		asm("move.l %%a7, %0" : "=r" (val));
		current_running_process->stack = val;
	}

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
			rtx_dbug_outs((CHAR *)"rtx: Love is sooo much .... CHARLIE !\r\n");
			// select the next process
			struct PCB* to_be_run = ready_queue[i];					
			
			// set state to running
			to_be_run->state = STATE_RUNNING;
			
			// update ready_queue and PCB data thingy <3
			current_running_process = to_be_run;
			ready_queue[i] = to_be_run->next;
			to_be_run->next = 0;

			val = current_running_process->stack;
			
			rtx_dbug_outs((CHAR *)"rtx: Love is sooo much .... CHARLIE !\r\n");
			
			stack_pointer_switcher(val-8);
			//asm("move.l %0, %%d0" : : "r" (val-8));
			//asm( "TRAP #0" );
		
			rtx_dbug_outs((CHAR *)"rtx: Love is sooo much .... CHARLIE !\r\n");
		
			int newVal = 0;
			asm("move.l %%a7, %0" : "=r" (newVal));

			break;
		}
	}
	rtx_dbug_outs((CHAR *)"rtx: after the scheduling loop the loop\r\n");
}

void stack_pointer_switcher()
{
	asm("move.l %d0, %a7");
	asm("move.l 8(%a6), %a6");
	//asm("move.l 8(%a6), %a7");
	//asm("move.l 8(%a6), %a6");
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
