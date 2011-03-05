#include "rtx_inc.h"
#include "memory.h"

UINT32 *free_blocks;
UINT32 *used_blocks = NULL;

struct queue ready_queue;
struct queue blocked_queue [6];

extern UINT32* mem_end; 

UINT8 next (struct queue q) {
	UINT8* temp = q.pid[current];
	q.current = (q.current + 1) % 6;
	return temp;
}

void add (struct queue q, UINT8* pid) {
	*(q.pid[end]) = pid;
	q.end = (q.end + 1) % 6;
}



struct process p [6] ;

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

		p[i].id = process_start;
		*(p[i].id) = g_test_proc[i].pid;
		p[i].state = process_start + 1;
		*(p[i].state) = 0;	//random val, assign meaning later (0 = ready, 1 = running, 2 = blocked?)
		p[i].priority = process_start + 2;
		*(p[i].priority) = g_test_proc[i].priority;
		p[i].pc = g_test_proc[i].entry; //point pc to entry point of code
		p[i].stack = process_start + 4;

		add(ready_queue, p[i].id); //add process to ready queue

		process_start = process_start + 4 + g_test_proc[i].sz_stack/4;
	}

	
}
