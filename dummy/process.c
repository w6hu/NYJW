#include "process.h"

// global variable to keep track of the current process that is running
struct PCB* current_running_process = 0;
struct PCB* prev_running_process = 0;

extern struct PCB p [NUM_PROCESS];

extern struct PCB* ready_queue[5];
extern struct PCB* blocked_queue[2];

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
	pcb_null_process->id = NULL_ID;
	pcb_null_process->priority = 4;
	pcb_null_process->stack = process_start;
	pcb_null_process->returning = FALSE;
	pcb_null_process->waiting_on = -1;
		
	int val;
	//back up a7
	int original_a7;
	asm("move.l %%a7, %0" : "=r" (original_a7));	
	val = pcb_null_process->stack;
	asm("move.l %0, %%a7" : : "r" (val));
	val = null_process;			
	asm("move.l %0, %%d0" : : "r" (val));
	asm("move.l %d0, -(%a7)");
	val = 4;			
	asm("move.w %0, %%d0" : : "r" (val));
	asm("move.w %d0, -(%a7)");
	val = 16512;			
	asm("move.w %0, %%d0" : : "r" (val));
	asm("move.w %d0, -(%a7)");
	pcb_null_process->stack -= 8;

	//restore a7
	asm("move.l %0, %%a7" : : "r" (original_a7));
		
	// initialize the process to the correct ready queue
	put_to_ready(pcb_null_process);
	pcb_null_process->state = STATE_NEW;	
}

int release_processor_kuma_san()
{	
	// set state and put the process to ready
	if(current_running_process->state == STATE_RUNNING) {
		put_to_ready (current_running_process);
	}

	
	schedule_next_process_neko_san();
	
	//other wise just return to the calling process and continue
	return RTX_SUCCESS;
}

void schedule_next_process()
{
	int val = CALLER_SCHEDULER;
	asm("move.l %0, %%d0" : : "r" (val));
	asm( "TRAP #0" );
}

void schedule_next_process_neko_san()
{
	//rtx_dbug_outs((CHAR *)"scheduled next process neko-san \r\n");
	// look for the next process.
	// if nothing is selected, the null 
	// process is there at your service.
	int i=0;
	for(i; i<5; i++)
	{
		if(ready_queue[i] != NULL)
		{	
			if(current_running_process->id == ready_queue[i]->id)
				continue;
			
			prev_running_process = current_running_process;
			
			// select the next process
			current_running_process = ready_queue[i];					
			ready_queue[i] = current_running_process->next;
			current_running_process->next = NULL;
			
			stack_pointer_switcher();			

			break;
		}
	}
}

VOID stack_pointer_switcher( VOID )
{
	asm("move.l %a6, -(%a7)");
	
	// we need a swtich statement to see what the calling function is,
	// since all other primitives are going to be calling this thing now
	
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
		// as this hack simply jump out of the trap, so 
		// need to reset automic here
		
		current_running_process->state = STATE_RUNNING;
		asm("rte");
	}
	
	current_running_process->state = STATE_RUNNING;
	asm("move.l (%a7)+, %a6");	
}

VOID trap_call_animal( VOID )
{
	// set automic here by disabling the interrupt
    // asm( "move.w #0x2700,%sr" );
	
	// get the caller id
	int val;
	int parm1;
	int parm2;
	int parm3;
	int return_val;
	
	asm("move.l %%d0, %0" : "=r" (val));
	// giant swtich to decide where to go
	if(val == CALLER_SCHEDULER)
	{
		schedule_next_process_neko_san();
	}
	else if(val == CALLER_RELEASE_PROCESSOR)
	{
		release_processor_kuma_san();
	}
	else if(val == CALLER_RELEASE_MEMORY_BLOCK)
	{
		asm("move.l +96(%%a7), %0" : "=r" (parm1));
		return_val = s_release_memory_block_yishi((void*)parm1);	
		asm("move.l %0, +96(%%a7)" : : "r" (return_val));		
	}
	else if(val == CALLER_REQUEST_MEMORY_BLOCK)
	{
		asm("move.l +96(%%a7), %0" : "=r" (parm1));
		return_val = (int)s_request_memory_block_yishi(parm1);	
		asm("move.l %0, +96(%%a7)" : : "r" (return_val));	
	}
	else if(val == CALLER_SEND_MESSAGE)
	{
		asm("move.l +96(%%a7), %0" : "=r" (parm2));
		asm("move.l +100(%%a7), %0" : "=r" (parm1));
		return_val = send_message_jessie(parm1, (void*)parm2);	
		asm("move.l %0, +96(%%a7)" : : "r" (return_val));
	}
	else if(val == CALLER_RECEIVE_MESSAGE)
	{
		asm("move.l +96(%%a7), %0" : "=r" (parm2));
		asm("move.l +100(%%a7), %0" : "=r" (parm1));
		return_val = receive_message_jessie(parm1, parm2);	
		asm("move.l %0, +96(%%a7)" : : "r" (return_val));	
	}
	else if(val == CALLER_DELAYED_SEND)
	{
		asm("move.l +96(%%a7), %0" : "=r" (parm3));
		asm("move.l +100(%%a7), %0" : "=r" (parm2));
		asm("move.l +104(%%a7), %0" : "=r" (parm1));
		return_val = delayed_send_umi_san(parm1, (void*)parm2, parm3);	
		asm("move.l %0, +96(%%a7)" : : "r" (return_val));
	}
	else if(val == CALLER_GET_PRIORITY)
	{
		asm("move.l +96(%%a7), %0" : "=r" (parm1));
		return_val = get_process_priority_usagi_san(parm1);	
		asm("move.l %0, +96(%%a7)" : : "r" (return_val));	
	}
	else if(val == CALLER_SET_PRIORITY)
	{
		asm("move.l +96(%%a7), %0" : "=r" (parm2));
		asm("move.l +100(%%a7), %0" : "=r" (parm1));
		return_val = set_process_priority_yama_san(parm1, parm2);	
		asm("move.l %0, +96(%%a7)" : : "r" (return_val));
	}
	
	// reset automic here by enabling the interrupt
}

struct PCB* get_process_from_ID(int process_id)
{
	int i = 0;
	for (i; i < NUM_PROCESS; i++) {
		if (p[i].id == process_id) {
			return &(p[i]);
		}
	}	
	return NULL;
}

int get_process_number_from_ID(int process_id)
{
	int i = 0;
	if (process_id == KEYBOARD_INTERRUPT) {
		return NUM_PROCESS;
	}
	if (process_id == TIMER_INTERRUPT) {
		return NUM_PROCESS+1;
	}
	for (i; i < NUM_PROCESS; i++) {
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

int set_process_priority_yama_san(int process_ID, int priority)
{
	struct PCB* to_be_changed = get_process_from_ID(process_ID);
	if(to_be_changed == NULL)
		return RTX_FAILURE;  // false for failing to set it
	
	// sainity checks; only 0, 1, 2, 3, levels allowed
	if(priority >= 4 || priority < 0)
		return RTX_FAILURE;
		
	// if the process is running or blocked, we don't need to 
	// change any queue, as it is not in the ready queue
	if(to_be_changed->state == STATE_READY)
	{
		remove_from_ready(to_be_changed);
		to_be_changed->priority = priority;
		put_to_ready(to_be_changed);
	}
	
	to_be_changed->priority = priority;
	return RTX_SUCCESS;
}

int process_exists(int process_id)
{
	int i = 0;
	if (process_id == KEYBOARD_INTERRUPT || process_id == TIMER_INTERRUPT) {
		return TRUE;
	}
	for (i; i < NUM_PROCESS; i++) {
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
