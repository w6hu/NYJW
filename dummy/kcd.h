#ifndef _RTX_KCD_H_
#define _RTX_KCD_H_

#include "rtx_inc.h"
#include "init.h"

void kcd ();
void init_kcd (struct PCB* pcb_kcd, UINT32* stackPtr);
int handle_time_string();
void register_command(int registering_process, char registering_command);

#endif
