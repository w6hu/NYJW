#include "init.h"
//#include "dbug.h"
#include <stdio.h>

UINT32 *free_blocks;
UINT32 *used_blocks = NULL;

extern UINT32* mem_end; 
extern struct PCB* ready_queue[5];
extern struct PCB* blocked_queue[1];

void put_to_ready(struct PCB* p)
{
	// get the process from process_id
	int priority = -1;
	struct PCB* temp = NULL;

	// get the priority of the process, 
	// hopefully it is between 0 and 4, otherwise we got array out of bound
	priority = p->priority;
	// get the corresponding ready queue from the pritority
	temp = ready_queue[priority];
	p->state = STATE_READY;
	if(temp == NULL)
	{
		ready_queue[priority] = p;
		p->next = NULL;	
		return;
	}
	
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
}

void put_to_blocked(int waiting_on, struct PCB* p)
{	
	// 0 is the message blocked queue

	// theoraticlaly shouldn't get here
	if (waiting_on < 0) {
		return;
	}
	// set the status in PCB
	p->waiting_on = waiting_on;
	p->state = STATE_BLOCKED;
	
	int priority = -1;
	struct PCB* temp = NULL;
	
	//remove_from_ready(p);
	p->next = NULL;

	priority = p->priority;
	// get the corresponding blocked queue	
	temp = blocked_queue[waiting_on];
	
	if(temp == NULL)
	{
		blocked_queue[waiting_on] = p;		
		return;
	}
	
	while(temp->next != NULL)
	{
		// if the process is alreay in there, don't bother doing anything (sanity check)
		if(temp == p)
			return;

		temp = temp->next;
	}
		
	// add the process into the corresponding queue and update the PCB
	temp->next = p;
}

void remove_from_ready(struct PCB* p) {
	if (ready_queue[p->priority] == p) {
		ready_queue[p->priority] = p->next;
	} else {
		struct PCB* queue = ready_queue [p->priority];
		while (queue->next != NULL) {
			if(queue->next == p) {
				queue->next = p->next;
				break;
			} else {
				queue = queue->next;
			}
		}
	}
}

void remove_from_blocked(int waiting_on, struct PCB* p) {
	// theoraticlaly shouldn't get here
	if (waiting_on < 0) {
		return;
	}
	
	//Assume if a process is removed from the ready-queue, we are moving the first one
	int i = 0;
	
	// reset the states in PCB
	p->state = STATE_READY;
	p->waiting_on = -1;
	
	if (blocked_queue [waiting_on] == p) {
		blocked_queue [waiting_on] = p->next;
	} else {
		struct PCB* queue = blocked_queue [waiting_on];
		while (queue->next != NULL) {
			if(queue->next == p) {
				queue->next = p->next;
				break;
			} else {
				queue = queue->next;
			}
		}
	}
	p->next = NULL;
	put_to_ready(p);
}


