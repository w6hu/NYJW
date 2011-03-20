#ifndef _RTX_PRIORITY_H_
#define _RTX_PRIORITY_H_

#include "rtx_inc.h"
#include "init.h"
#include "rtx.h"

void priority_modifier();
void init_pm(struct PCB* pcb_pm, UINT32* stackPtr);

#endif