#include "process.h"

// global variable to keep track of the current process that is running
struct PCB* current_running_process = 0;
struct PCB* imma_epeen_process = 0;

extern struct PCB* ready_queue[5];
extern struct PCB* blocked_queue[6];

// need to import the ready_queue here ~

int release_processor_kuma_san()
{
	
	//int val = current_running_process->stack;
	//asm("move.l %0, %%a7" : : "r" (val));
	int val;
	asm("move.l %%a7, %0" : "=r" (val));
	
	
		asm("move.l %a0, -(%a7)");
		asm("move.l %a1, -(%a7)");
		asm("move.l %a2, -(%a7)");
		asm("move.l %a3, -(%a7)");
		asm("move.l %a4, -(%a7)");
		asm("move.l %a5, -(%a7)");
		asm("move.l %d0, -(%a7)");
		asm("move.l %d1, -(%a7)");
		asm("move.l %d2, -(%a7)");
		asm("move.l %d3, -(%a7)");
		asm("move.l %d4, -(%a7)");
		asm("move.l %d5, -(%a7)");
		asm("move.l %d6, -(%a7)");
		asm("move.l %d7, -(%a7)");
	
	
	asm("move.l %%a7, %0" : "=r" (val));
	current_running_process->stack = val;

	//asm("rts");
	//rtx_dbug_outs((CHAR *)"rtx: Kuma san Konnichi ha ! !!\r\n");
	
	// store the a7 to PCB stack
	//asm("move.l %%a7, %0" : "=r" (val));
	//current_running_process->stack = val;
	
	// save PSW
	//asm("move.w %sr, %d0");
	UINT16 srVal;
	asm("move.w %%sr, %0" : "=r" (srVal));
	current_running_process->psw = srVal;
	
	// set the flag so that it would go to selecting next process
	current_running_process->returning = FALSE;
	
	// save PC
	asm("lea 8(%pc), %a0");
	asm("move.l %%a0, %0" : "=r" (val));
	//rtx_dbug_outs((CHAR *)"rtx: Yamete~~~\r\n");
	current_running_process->pc = val;
	//rtx_dbug_outs((CHAR *)"rtx: Tasukete~~~\r\n");
	
	val = current_running_process->stack;
	asm("move.l %0, %%a7" : : "r" (val));
	//rtx_dbug_outs((CHAR *)"rtx: A7:\r\n");
	asm("move.l %%a7, %0" : "=r" (val));
	current_running_process->stack = val;
	
	
	//rtx_dbug_outs((CHAR *)"rtx: Kuma san Konban ha !!\r\n");
		
	// if the process is not returning, then we look for another one
	if(current_running_process->returning == FALSE)
	{
		//rtx_dbug_outs((CHAR *)"rtx: Kuma san kowareta~~~ \r\n");
		if(current_running_process->state == STATE_READY)
			put_to_ready(current_running_process);
		else
			put_to_blocked(current_running_process->waiting_on, current_running_process);
	
		schedule_next_process();
	}
	
		current_running_process->state2 = FINISHED;
		//rtx_dbug_outs((CHAR *) "Kuma San Oyasumi ~ =3=/~\r\n");		
	
	//other wise just return to the calling process and continue
	return RTX_SUCCESS;
}

// TODO right now it simply choose the highest priority process, we need
// some way to override this behaviour
void schedule_next_process()
{
	//asm("rts");
	// look for the next process.
	// if nothing is selected, the null 
	// process is there at your service.
	int i=0;
	rtx_dbug_outs((CHAR *)"rtx: before the scheduling loop\r\n");
	for(i; i<5; i++)
	{
	//rtx_dbug_outs((CHAR *)"looping in scheduler\r\n");
		if(ready_queue[i] != NULL)
		{
		
			struct PCB* to_be_run = ready_queue[i];

			current_running_process = to_be_run;
			ready_queue[i] = to_be_run->next;
			to_be_run->next = 0;
			
			to_be_run->returning = TRUE;
		
			current_running_process->state2 = RUNNING;		
			
			int val = current_running_process->stack;
			
				asm("move.l %0, %%a7" : : "r" (val));
				asm("move.l (%a7)+, %d7");
				asm("move.l (%a7)+, %d6");
				asm("move.l (%a7)+, %d5");
				asm("move.l (%a7)+, %d4");
				asm("move.l (%a7)+, %d3");
				asm("move.l (%a7)+, %d2");
				asm("move.l (%a7)+, %d1");
				asm("move.l (%a7)+, %d0");
				asm("move.l (%a7)+, %a5");
				asm("move.l (%a7)+, %a4");
				asm("move.l (%a7)+, %a3");
				asm("move.l (%a7)+, %a2");
				asm("move.l (%a7)+, %a1");
				asm("move.l (%a7)+, %a0");
			
			asm("move.l %%a7, %0" : "=r" (val));
			current_running_process->stack = val;
			//rtx_dbug_outs((CHAR *)"rtx: The side that has a pot of gold! ~!\r\n");				
			//rtx_dbug_outs((CHAR *)"rtx: How can you NOT do this to me ~ I am lonely <3 1!\r\n");
			// restore the PWS
			val = current_running_process->psw;
			asm("move.l %0, %%d0" : : "r" (val));
			asm("move.w %d0, %sr");
			//rtx_dbug_outs((CHAR *)"rtx: How can you NOT do this to me ~ I am lonely <3 2!\r\n");
			// restore the PC
			val = current_running_process->pc;
			asm("move.l %0, %%a1" : : "r" (val));
			rtx_dbug_outs((CHAR *)"rtx: How can you NOT do this to me ~ I am lonely <3 3!\r\n");
		/*
			int last; //= tempEnd%10;
			int remain = val;
			//int i = 0; 
			while (remain != 0) {
				//rtx_dbug_out_char((CHAR)(last+48));
				last = remain%10;
				remain = remain/10;
				rtx_dbug_out_char((CHAR)(last+48));            
			}
			rtx_dbug_outs((CHAR *) "\r\n");			
			*/
			asm("jmp (%a1)");
		
			// this is hopefully not run at all ;
			rtx_dbug_outs((CHAR *)"rtx: SOMETHING IS WRONG, SHOULD NOT REACH HERE!\r\n");
			while(1);
			break;
		}
	}
	rtx_dbug_outs((CHAR *)"rtx: after the scheduling loop the loop\r\n");
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
			return (current_running_process->waiting_on == process_ID);
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

