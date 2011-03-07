#include "init.h"
#include "dbug.h"

UINT32 *free_blocks;
UINT32 *used_blocks = NULL;

extern UINT32* mem_end; 

void put_to_ready(struct PCB* p)
{
	rtx_dbug_outs((CHAR *)"rtx: Inside Put to ready\r\n");
	// get the process from process_id
	int priority = -1;
	struct PCB* temp = NULL;

	// if the process is in the block queue, remove it from the block queue
	//remove_from_blocked(p);

	// get the priority of the process, 
	// hopefully it is between 0 and 4, otherwise we got array out of bound
	priority = p->priority;
	// get the corresponding ready queue from the pritority
	temp = ready_queue[priority];
	
	if(temp == NULL)
	{
		rtx_dbug_outs((CHAR *)"rtx: breaking the first time ... yamete~\r\n");
		ready_queue[priority] = p;
		p->next = NULL;
		
		return;
	}
	
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
	// set the status in PCB
	p->waiting_on = waiting_on;
	p->state = STATE_BLOCKED;
	
	// process starts from 1, but array starts from 0;
	waiting_on--;
	// get the process from process_id
	int priority = -1;
	struct PCB* temp = NULL;

	//remove_from_ready(p);

	// get the priority of the process, 
	// hopefully it is between 0 and 4, otherwise we got array out of bound
	
	rtx_dbug_outs((CHAR *)"rtx: Inside of put_to_blocked\r\n");
	
	priority = p->priority;
	// get the corresponding blocked queue
	rtx_dbug_outs((CHAR *)"rtx: Inside of put_to_blocked2\r\n");
	rtx_dbug_out_char(waiting_on + 48);
	temp = blocked_queue[waiting_on];
	
	rtx_dbug_outs((CHAR *)"rtx: Finding the tail of blocked queue\r\n");
	
	while(temp != NULL && temp->next != NULL)
	{

		// if the process is alreay in there, don't bother doing anything (sanity check)
		if(temp == p)
			return;

		temp = temp->next;
	}
	
	rtx_dbug_outs((CHAR *)"rtx: Found the tail of blocked queue\r\n");
	
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

void remove_from_blocked(int waiting_on, struct PCB* p) {
	rtx_dbug_outs((CHAR *)"rtx: Remove from blocked\r\n");
	
	waiting_on--;
	
	//Assume if a process is removed from the ready-queue, we are moving the first one
	int i = 0;
	
	// reset the states in PCB
	p->state = STATE_READY;
	p->waiting_on = -1;
	struct PCB* queue = blocked_queue [waiting_on];
	while (queue != NULL) {
		if(queue == p) {
			queue->next = p->next;
			rtx_dbug_outs((CHAR *)"rtx: Found the process to remove\r\n");
			break;
		}
		else {
			queue = queue->next;
		}
	}
	
	put_to_ready(p);
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


//only used for init when the val for a7 is not known, DO NOT USE ANYWHERE ELSE
void push_down_first_night (struct PCB* p, UINT32 val) {
	val = p->stack;
	asm("move.l %0, %%a7" : : "r" (val));
	asm("move.l %d0, %a7");
	
	int i = 0;
	val = 0;
	for (i; i < 16; i++) {
		push_test(val);
	}
	asm("move.l %%a7, %0" : "=r" (val));
	p->stack = val;
}

UINT32 pop_test () {
	UINT32 val;
	asm("move.l -(%%a7), %0" : "=r" (val));
	//asm("move.l %%a7, %0" : "=r" (val));
	return val;
}

void push_test (UINT32 val) {
	asm("move.l %a7, %d0");
	rtx_dbug_outs((CHAR *)"rtx: almost there \r\n");
	asm("move.l %d0, %0" : "=r" (val));
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
	rtx_dbug_outs((CHAR *)"rtx: push test start\r\n");
	asm("move.l #0, %d0");
	asm("move.l %d0, (%a7)");
	rtx_dbug_outs((CHAR *)"rtx: pushing~~ \r\n");
	asm("move.l %a7, %d0");
	rtx_dbug_outs((CHAR *)"rtx: almost there \r\n");
	asm("move.l %d0, %0" : "=r" (val));
	last; //= tempEnd%10;
	remain = val;
	//int i = 0; 
	while (remain != 0) {
		//rtx_dbug_out_char((CHAR)(last+48));
		last = remain%10;
		remain = remain/10;
		rtx_dbug_out_char((CHAR)(last+48));            
	}
	rtx_dbug_outs((CHAR *) "\r\n");		
	
	asm("add.l #8, %d0");
	asm("move.l %d0, %0" : "=r" (val));
	last; //= tempEnd%10;
	remain = val;
	//int i = 0; 
	while (remain != 0) {
		//rtx_dbug_out_char((CHAR)(last+48));
		last = remain%10;
		remain = remain/10;
		rtx_dbug_out_char((CHAR)(last+48));            
	}
	rtx_dbug_outs((CHAR *) "\r\n");		
	
	rtx_dbug_outs((CHAR *)"rtx: a little bit more \r\n");
	asm("move.l %d0, %a7");
	rtx_dbug_outs((CHAR *)"rtx: push test end\r\n");
}
