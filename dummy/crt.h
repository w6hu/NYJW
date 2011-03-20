#ifndef _RTX_KCD_H_
#define _RTX_KCD_H_

#include "rtx_inc.h"
#include "init.h"
#include "process.h"


void crt ();
void init_crt (struct PCB* pcb_crt, UINT32* stackPtr);

#endif
