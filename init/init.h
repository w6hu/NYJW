#ifndef _RTX_INIT_H_
#define _RTX_INIT_H_

#include "../dummy/rtx.h"
#include "../dummy/rtx_test.h"
#include "../memory/memory.h"

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
};

void put_to_ready(struct PCB* p);
void put_to_blocked(int waiting_on, struct PCB* p);
void remove_from_ready(struct PCB* p);
void remove_from_blocked(struct PCB* p);

UINT32 pop (struct PCB* p);
void push (struct PCB* p, UINT32 val);

void init ();

#endif
