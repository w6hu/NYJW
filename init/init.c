#include "rtx_inc.h"
#include "memory.h"

UINT32 *free_blocks;
UINT32 *used_blocks = NULL;


extern UINT32* mem_end; 

void put_to_ready(PCB* p)
{
	// get the process from process_id
	int priority = -1;
	PCB* temp = NULL;

	// if the process is in the block queue, remove it from the block queue
	remove_from_blocked(p);

	// get the priority of the process, 
	// hopefully it is between 0 and 4, otherwise we got array out of bound
	priority = p->priority;
	// get the corresponding ready queue from the pritority
	temp = ready_queue[priority];
	
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
	p->state = STATE_READY;
}

void put_to_blocked(int waiting_on, PCB* p)
{
	// get the process from process_id
	int priority = -1;
	PCB* temp = NULL;

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

void remove_from_ready(PCB* p) {
	//Assume if a process is removed from the ready-queue, we are moving the first one
	PCB* queue = ready_queue [p->priority];
	if (queue != NULL && queue = p)
		queue->next = p->next;
}

void remove_from_blocked(PCB* p) {
	//Assume if a process is removed from the ready-queue, we are moving the first one
	for (int i = 0; i < 6; i++) {
		PCB* queue = blocked_queue [i];
		if (queue != NULL && queue = p) {
			queue->next = p->next;
			return;
		}
	}
}


struct PCB* ready_queue [4];
struct PCB* blocked_queue [6];
struct PCB p [6] ;

<<<<<<< HEAD
UINT32 pop (struct PCB p) {
	UINT32 temp = *(p.stack);
	p.stack = p.stack - 1;
	return temp;
}

void push (struct PCB p, UINT32 val) {
	*(p.stack) = val;
	p.stack = p.stack + 1;
=======
UINT32 pop (struct process* p) {
	UINT32 temp = *(p->stack);
	p->stack = p->stack - 1;
	return temp;
}

void push (struct process* p, UINT32 val) {
	*(p->stack) = val;
	p->stack = p->stack + 1;
}

void init () {
	free_blocks = initBlock(NUM_MEM_BLKS);
	int i = 0;
	UNIT32* process_start = mem_end;

	for (i; i < 6; i++) {
		p[i].next = process_start;
		*p[i].next = NULL;
		p[i].id = process_start + 1;
		*(p[i].id) = g_test_proc[i].pid;
		p[i].state = process_start + 2;
		*(p[i].state) = STATE_READY;
		p[i].priority = process_start + 3;
		*(p[i].priority) = g_test_proc[i].priority;
		p[i].pc = g_test_proc[i].entry; //point pc to entry point of code
		p[i].stack = process_start + 5;

		ready_queue[p[i].priority]->next = p[i];

		process_start = process_start + 5 + g_test_proc[i].sz_stack/4;
	}

	
}
