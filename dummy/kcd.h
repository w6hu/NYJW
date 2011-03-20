#ifndef _RTX_CRT_H_
#define _RTX_CRT_H_

#include "rtx_inc.h"
#include "init.h"
#include "rtx.h"

void kcd ();
void init_kcd (struct PCB* pcb_kcd, UINT32* stackPtr);
int handle_time_string();

#endif
