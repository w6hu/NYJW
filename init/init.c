#include "init.h"
#include "../dummy/dbug.h"

extern test_fixture_t g_test_fixture;
extern test_proc_t g_test_proc[NUM_TEST_PROCS];

UINT32 *free_blocks;
UINT32 *used_blocks = NULL;


extern UINT32* mem_end; 

struct PCB* ready_queue[5];
struct PCB* blocked_queue[6];
struct PCB p [6] ;

void put_to_ready(struct PCB* p)
{
	// get the process from process_id
	int priority = -1;
	struct PCB* temp = NULL;

	// if the process is in the block queue, remove it from the block queue
	remove_from_blocked(p);

	// get the priority of the process, 
	// hopefully it is between 0 and 4, otherwise we got array out of bound
	priority = p->priority;
	// get the corresponding ready queue from the pritority
	temp = ready_queue[priority];
	
	while(temp != NULL && temp->next != NULL)
	{

		rtx_dbug_outs((CHAR *)"rtx: Infinite Loop in put to ready\r\n");
		// if the process is alreay in there, don't bother doing anything (sanity check)
		if(temp == p)
			return;

		temp = temp->next;
	}
	
	// at this point, temp should be the tail of our lovely queue.
	// add the process into the corresponding queue and update the PCB
	temp->next = p;
	p->next = NULL;
	p->state = STATE_READY;
}

void put_to_blocked(int waiting_on, struct PCB* p)
{
	// get the process from process_id
	int priority = -1;
	struct PCB* temp = NULL;

	remove_from_ready(p);

	// get the priority of the process, 
	// hopefully it is between 0 and 4, otherwise we got array out of bound
	priority = p->priority;
	// get the corresponding blocked queue
	temp = blocked_queue[waiting_on];
	
	while(temp != NULL && temp->next != NULL)
	{

		// if the process is alreay in there, don't bother doing anything (sanity check)
		if(temp == p)
			return;

		temp = temp->next;
	}
	
	// at this point, temp should be the tail of our lovely queue.
	// add the process into the corresponding queue and update the PCB
	temp->next = p;
	p->next = NULL;
	p->state = STATE_BLOCKED;
}

void remove_from_ready(struct PCB* p) {
	//Assume if a process is removed from the ready-queue, we are moving the first one
	struct PCB* queue = ready_queue [p->priority];
	if (queue != NULL && queue == p)
		queue->next = p->next;
}

void remove_from_blocked(struct PCB* p) {
	//Assume if a process is removed from the ready-queue, we are moving the first one
	int i = 0;
	for ( i; i < 6; i++) {
		struct PCB* queue = blocked_queue [i];
		if (queue != NULL && queue == p) {
			queue->next = p->next;
			return;
		}
	}
}



UINT32 pop (struct PCB* p) {
	UINT32 temp = *(p->stack);
	p->stack = p->stack - 1;
	return temp;
}

void push (struct PCB* p, UINT32 val) {
	*(p->stack) = val;
	p->stack = p->stack + 1;
}

void init (test_fixture_t g_test_fixture, test_proc_t g_test_proc[]) {
    rtx_dbug_outs((CHAR *)"rtx: Entering init()\r\n");
	free_blocks = initBlock(NUM_MEM_BLKS);
	rtx_dbug_outs((CHAR *)"rtx: Created Memory blocks\r\n");
	/*
	int i = 0;
	UINT32* process_start = mem_end;

	for (i; i < 6; i++) {
		rtx_dbug_outs((CHAR *)"rtx: Infinite Loop\r\n");
		p[i].next = NULL;
		p[i].id = g_test_proc[i].pid;
		rtx_dbug_outs((CHAR *)"rtx: Got PID\r\n");
		p[i].state = STATE_READY;
		p[i].priority = g_test_proc[i].priority;
		p[i].psw = 0;   // assuming 0 is the nomal initial state ... eh ?
		rtx_dbug_outs((CHAR *)"rtx: Getting pc\r\n");
		p[i].pc = g_test_proc[i].entry; //point pc to entry point of code
		rtx_dbug_outs((CHAR *)"rtx: pc set\r\n");
		p[i].stack = process_start; // where exactly is the process stack ?
		
		// initialize the process to the correct ready queue
		//put_to_ready(&(p[i]));

		process_start = process_start + g_test_proc[i].sz_stack/4;
	}*/

	
}
