#ifndef _RTX_INIT_H_
#define _RTX_INIT_H_

#include "rtx_inc.h"

//Built based on structure from test dummy procs
struct PCB {
	UINT16 psw;
	
	UINT32 pc;	
	UINT32 stack;

	struct PCB* next;
	int id;
	int state;			
	int priority;
	int returning;
	int waiting_on;
	int state2;
};

void put_to_ready(struct PCB* p);
void put_to_blocked(int waiting_on, struct PCB* p);
void remove_from_ready(struct PCB* p);
void remove_from_blocked(int waiting_for, struct PCB* p);

#endif
