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

void init_null_process( struct PCB* null_process, UINT32* process_start)
{
/*
	VOID   (*entry)();
	entry = null_process;
	
	rtx_dbug_outs((CHAR *)"rtx: Initializing the null process\r\n");
	null_process->next = NULL;
	null_process->id = -2;
	null_process->state = STATE_READY;
	null_process->priority = 0;
	null_process->psw = 9984;   // assuming 9984 is the nomal initial state ... eh ?
	null_process->pc = entry; //point pc to entry point of code
	null_process->stack = process_start; // where exactly is the process stack ?
	null_process->returning = FALSE;
	null_process->waiting_on = -1;
		
	int j = 0;
	for (j; j < 14; j++) {
		push(null_process, 0);
	}
		
	// initialize the process to the correct ready queue
	put_to_ready(null_process);
	*/
}

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
	//asm("move.l %%a6, %0" : "=r" (val));	//TODO a6 is not backup or restore a6.... but we should
	//push(val);
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

	rtx_dbug_outs((CHAR *)"rtx: Kuma san Konbanha ! !!\r\n");
	
	// store the a7 to PCB stack
	//asm("move.l %%a7, %0" : "=r" (val));
	//current_running_process->stack = val;
	
	// save PSW
	asm("move.w %sr, %d0");
	asm("move.w %%d0, %0" : "=r" (val));
	current_running_process->psw = val;
	
	// set the flag so that it would go to selecting next process
	current_running_process->returning = FALSE;
	
	// save PC
	asm("lea 8(%pc), %a0");
	asm("move.l %%a0, %0" : "=r" (val));
	current_running_process->pc = val;
	
	int last;
	int remain = val;
	//int i = 0; 
	while (remain != 0) {
		//rtx_dbug_out_char((CHAR)(last+48));
		last = remain%10;
		remain = remain/10;
		rtx_dbug_out_char((CHAR)(last+48));            
	}
	rtx_dbug_outs((CHAR *) "\r\n");	
	
	rtx_dbug_outs((CHAR *)"rtx: Kuma san Konnichiha !!\r\n");
	last; //= tempEnd%10;
	remain = current_running_process->id;
	//int i = 0; 
	while (remain != 0) {
		//rtx_dbug_out_char((CHAR)(last+48));
		last = remain%10;
		remain = remain/10;
		rtx_dbug_out_char((CHAR)(last+48));            
	}
	rtx_dbug_outs((CHAR *) "\r\n");	
	
	// if the process is not returning, then we look for another one
	if(current_running_process->returning == FALSE)
	{
		if(current_running_process->state == STATE_READY)
			put_to_ready(current_running_process);
		else
			put_to_blocked(current_running_process->waiting_on, current_running_process);
	
		schedule_next_process();
	}
	
	rtx_dbug_outs((CHAR *) "Kuma San Oyasumi ~ =3=/~\r\n");		
	
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
			rtx_dbug_outs((CHAR *)"rtx: Love is sooo much .... CHARLIE !\r\n");
			// select the next process
			struct PCB* to_be_run = ready_queue[i];

			// update ready_queue and PCB data thingy <3
			current_running_process = to_be_run;
			ready_queue[i] = to_be_run->next;
			to_be_run->next = 0;
			to_be_run->returning = FALSE;
			rtx_dbug_outs((CHAR *)"rtx: One side of the rainbow ~!\r\n");	
			
			// get the a7 value before we start popping, as pop would use a7 to do its bidding 
			int val = 0;
			//val = current_running_process->stack;
			//asm("move.l %0, %%a7" : : "r" (val));
			
			// restore the register d7 ~ d0, a7 ~ a0
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
			
			rtx_dbug_outs((CHAR *)"rtx: Somewhere over the Rainbow ~ Skys are blue! ~!\r\n");				
			
		//	val = pop();		  // TODO a6 is some magical register that, if modified, crash the OS ....
		//	if(val != 0)							  // a hack.  Since the frame pointer can never be 0, we initialize it 	
		//		asm("move.l %0, %%a6" : : "r" (val)); // to 0, and if it is a 0, don't bother pushing it onto a6 ^^; 
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
			rtx_dbug_outs((CHAR *)"rtx: The side that has a pot of gold! ~!\r\n");				
			rtx_dbug_outs((CHAR *)"rtx: How can you NOT do this to me ~ I am lonely <3 1!\r\n");
			// restore the PWS
			val = current_running_process->psw;
			asm("move.l %0, %%d0" : : "r" (val));
			asm("move.w %d0, %sr");
			rtx_dbug_outs((CHAR *)"rtx: How can you NOT do this to me ~ I am lonely <3 2!\r\n");
			// restore the PC
			val = current_running_process->pc;
			asm("move.l %0, %%a1" : : "r" (val));
			rtx_dbug_outs((CHAR *)"rtx: How can you NOT do this to me ~ I am lonely <3 3!\r\n");
		
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
