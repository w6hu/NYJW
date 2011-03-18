#ifndef _RTX_INIT_H_
#define _RTX_INIT_H_

#include "rtx_inc.h"

struct queue {
	UINT8 pid [6];
	UINT32* current;
	UINT32* end; //loc to add next val
};

//Built based on structure from test dummy procs
struct PCB {	
	UINT32 stack;

	struct PCB* next;
	int id;
	int state;			
	int priority;
	int returning;
	int waiting_on;
};

void put_to_ready(struct PCB* p);
void put_to_blocked(int waiting_on, struct PCB* p);
void remove_from_ready(struct PCB* p);
void remove_from_blocked(int waiting_for, struct PCB* p);
void remove_first_from_blocked(int index);

UINT32 pop (struct PCB* p);
void push (struct PCB* p, UINT32 val);

UINT32 pop_test ();
void push_test (UINT32 val);
void push_down_first_night (struct PCB* p, UINT32 val);

#endif
