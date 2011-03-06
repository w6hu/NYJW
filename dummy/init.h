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
	UINT16 psw;
	
	UINT32* pc;	
	UINT32* stack;

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
void remove_from_blocked(struct PCB* p);

UINT32 pop (struct PCB* p);
void push (struct PCB* p, UINT32 val);

struct PCB* ready_queue[5];
struct PCB* blocked_queue[6];

#endif
